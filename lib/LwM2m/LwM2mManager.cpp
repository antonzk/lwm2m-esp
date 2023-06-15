#include "LwM2mManager.h"

bool LwM2mManager::init()
{
  memset(&data, 0, sizeof(client_data_t));
  data.addressFamily = AF_INET;

  if (!server)
  {
    server = (AF_INET == data.addressFamily ? DEFAULT_SERVER_IPV4 : DEFAULT_SERVER_IPV6);
  }

  /*
     *This call an internal function that create an IPV6 socket on the port 5683.
     */
  fprintf(stdout, "Trying to bind LWM2M Client to port %s\r\n", localPort);

  data.sock = create_socket(localPort, data.addressFamily);
  if (data.sock < 0)
  {
    fprintf(stderr, "Failed to open socket: %d %s\r\n", errno, strerror(errno));
    //TODO return
  }
  /*
     * Now the main function fill an array with each object, this list will be later passed to liblwm2m.
     * Those functions are located in their respective object file.
     */
#ifdef WITH_TINYDTLS
  if (psk != NULL)
  {
    pskLen = strlen(psk) / 2;
    pskBuffer = (char *)malloc(pskLen);

    if (NULL == pskBuffer)
    {
      fprintf(stderr, "Failed to create PSK binary buffer\r\n");
      //TODO return
    }
    // Hex string to binary
    char *h = psk;
    char *b = pskBuffer;
    char xlate[] = "0123456789ABCDEF";

    for (; *h; h += 2, ++b)
    {
      char *l = strchr(xlate, toupper(*h));
      char *r = strchr(xlate, toupper(*(h + 1)));

      if (!r || !l)
      {
        fprintf(stderr, "Failed to parse Pre-Shared-Key HEXSTRING\r\n");
        //TODO return
      }

      *b = ((l - xlate) << 4) + (r - xlate);
    }
  }
#endif

  char serverUri[50];
  int serverId = 123;
#ifdef WITH_TINYDTLS
  sprintf(serverUri, "coaps://%s:%s", server, serverPort);
  strcpy(pskId, PSKID);
  strcpy(psk, PSK);
#else

  sprintf(serverUri, "coap://%s:%s", server, serverPort);
#endif
#ifdef LWM2M_BOOTSTRAP

  objArray[objectCount] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, bootstrapRequested);
#else
  objArray[objectCount] = get_security_object(serverId, serverUri, pskId, pskBuffer, pskLen, false);
#endif
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create security object\r\n");
    //TODO return
  }
  data.securityObjP = objArray[objectCount];
  objectCount++;

  objArray[objectCount] = get_server_object(serverId, "U", lifetime, false);
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create server object\r\n");
    //TODO return
  }
  objectCount++;

  objArray[objectCount] = get_object_device();
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create Device object\r\n");
    //TODO return
  }
  objectCount++;

  objArray[objectCount] = get_object_firmware();
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create Firmware object\r\n");
    //TODO return
  }
  objectCount++;

  objArray[objectCount] = get_object_location();
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create location object\r\n");
    //TODO return
  }
  objectCount++;

  objArray[objectCount] = get_object_conn_m();
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create connectivity monitoring object\r\n");
    //TODO return
  }
  objectCount++;

  objArray[objectCount] = get_object_conn_s();
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create connectivity statistics object\r\n");
    //TODO return
  }
  objectCount++;

  int instId = 0;
  objArray[objectCount] = acc_ctrl_create_object();
  if (NULL == objArray[objectCount])
  {
    fprintf(stderr, "Failed to create Access Control object\r\n");
    //TODO return
  }
  else if (acc_ctrl_obj_add_inst(objArray[objectCount], instId, 3, 0, serverId) == false)
  {
    fprintf(stderr, "Failed to create Access Control object instance\r\n");
    //TODO return
  }
  else if (acc_ctrl_oi_add_ac_val(objArray[objectCount], instId, 0, 0b000000000001111) == false)
  {
    fprintf(stderr, "Failed to create Access Control ACL default resource\r\n");
    //TODO return
  }
  else if (acc_ctrl_oi_add_ac_val(objArray[objectCount], instId, 999, 0b000000000000001) == false)
  {
    fprintf(stderr, "Failed to create Access Control ACL resource for serverId: 999\r\n");
    //TODO return
  }
  objectCount++;
  /*
     * The liblwm2m library is now initialized with the functions that will be in
     * charge of communication
     */

  // Add different smart objects
  //TODO return
  // addSmartObject(&objectCount);
  //not code below
  // objectCount++;

  lwm2mH = lwm2m_init(&data);
  if (NULL == lwm2mH)
  {
    fprintf(stderr, "lwm2m_init() failed\r\n");
    //TODO return
  }

#ifdef WITH_TINYDTLS
  data.lwm2mH = lwm2mH;
#endif

  /*
     * We configure the liblwm2m library with the name of the client - which shall be unique for each client -
     * the number of objects we will be passing through and the objects array
     */
  fprintf(stderr, "Here %d\r\n", objectCount);
  result = lwm2m_configure(lwm2mH, name, NULL, NULL, objectCount, objArray);
  fprintf(stderr, "No");
  if (result != 0)
  {
    fprintf(stderr, "lwm2m_configure() failed: 0x%X\r\n", result);
    //TODO return
  }

  /**
     * Initialize value changed callback.
     */
  init_value_change(lwm2mH);

  fprintf(stdout, "LWM2M Client \"%s\" started on port %s\r\n", name, localPort);
  fprintf(stdout, "> ");
  fflush(stdout);
  return true;
  /*
     * We now enter in a while loop that will handle the communications from the server
     */
}

void LwM2mManager::lifecycle()
{
  struct timeval tv;
  fd_set readfds;
  fprintf(stdout, "\n START\n");

  tv.tv_sec = 60;
  tv.tv_usec = 0;

  FD_ZERO(&readfds);
  FD_SET(data.sock, &readfds);
  FD_SET(STDIN_FILENO, &readfds);

  /*
         * This function does two things:
         *  - first it does the work needed by liblwm2m (eg. (re)sending some packets).
         *  - Secondly it adjusts the timeout value (default 60s) depending on the state of the transaction
         *    (eg. retransmission) and the time between the next operation
         */
  result = lwm2m_step(lwm2mH, &(tv.tv_sec));
  fprintf(stdout, " -> State: ");
  switch (lwm2mH->state)
  {
  case STATE_INITIAL:
    fprintf(stdout, "STATE_INITIAL\r\n");
    break;
  case STATE_BOOTSTRAP_REQUIRED:
    fprintf(stdout, "STATE_BOOTSTRAP_REQUIRED\r\n");
    break;
  case STATE_BOOTSTRAPPING:
    fprintf(stdout, "STATE_BOOTSTRAPPING\r\n");
    break;
  case STATE_REGISTER_REQUIRED:
    fprintf(stdout, "STATE_REGISTER_REQUIRED\r\n");
    break;
  case STATE_REGISTERING:
    fprintf(stdout, "STATE_REGISTERING\r\n");
    break;
  case STATE_READY:
    fprintf(stdout, "STATE_READY\r\n");
    break;
  default:
    fprintf(stdout, "Unknown...\r\n");
    break;
  }
  if (result != 0)
  {
    fprintf(stderr, "lwm2m_step() failed: 0x%X\r\n", result);
    if (previousState == STATE_BOOTSTRAPPING)
    {
#ifdef WITH_LOGS
      fprintf(stdout, "[BOOTSTRAP] restore security and server objects\r\n");
#endif
      prv_restore_objects(lwm2mH);
      lwm2mH->state = STATE_INITIAL;
    }
    else
    {
      //TODO return
    }
  }
#ifdef LWM2M_BOOTSTRAP
  update_bootstrap_info(&previousState, lwm2mH);
#endif
  /*
         * This part will set up an interruption until an event happen on SDTIN or the socket until "tv" timed out (set
         * with the precedent function)
         */
  result = select(FD_SETSIZE, &readfds, NULL, NULL, &tv);

  if (result < 0)
  {
    if (errno != EINTR)
    {
      fprintf(stderr, "Error in select(): %d %s\r\n", errno, strerror(errno));
    }
  }
  else if (result > 0)
  {
    uint8_t buffer[MAX_PACKET_SIZE];
    int numBytes;

    /*
             * If an event happens on the socket
             */
    if (FD_ISSET(data.sock, &readfds))
    {
      struct sockaddr_storage addr;
      socklen_t addrLen;

      addrLen = sizeof(addr);

      /*
                 * We retrieve the data received
                 */
      numBytes = recvfrom(data.sock, buffer, MAX_PACKET_SIZE, 0, (struct sockaddr *)&addr, &addrLen);

      if (0 > numBytes)
      {
        fprintf(stderr, "Error in recvfrom(): %d %s\r\n", errno, strerror(errno));
      }
      else if (0 < numBytes)
      {
        char s[INET6_ADDRSTRLEN];
        in_port_t port;

#ifdef WITH_TINYDTLS
        dtls_connection_t *connP;
#else
        connection_t *connP;
#endif
        if (AF_INET == addr.ss_family)
        {
          struct sockaddr_in *saddr = (struct sockaddr_in *)&addr;
          inet_ntop(saddr->sin_family, &saddr->sin_addr, s, INET6_ADDRSTRLEN);
          port = saddr->sin_port;
        }
        else if (AF_INET6 == addr.ss_family)
        {
          struct sockaddr_in6 *saddr = (struct sockaddr_in6 *)&addr;
          inet_ntop(saddr->sin6_family, &saddr->sin6_addr, s, INET6_ADDRSTRLEN);
          port = saddr->sin6_port;
        }
        fprintf(stderr, "%d bytes reeceivd from [%s]:%hu\r\n", numBytes, s, ntohs(port));
        //TODO check
        // connP = connection_find(data.connList, &addr, addrLen);
        connP = data.connList;
        if (connP != NULL)
        {
          /*
                         * Let liblwm2m respond to the query depending on the context
                         */
#ifdef WITH_TINYDTLS
          int result = connection_handle_packet(connP, buffer, numBytes);
          if (0 != result)
          {
            printf("error handling message %d\n", result);
          }
#else
          lwm2m_handle_packet(lwm2mH, buffer, numBytes, connP);
#endif
          conn_s_updateRxStatistic(objArray[6], numBytes, false);
        }
        else
        {
          fprintf(stderr, "received bytes ignored!\r\n");
        }
      }
    }
  }
}

void LwM2mManager::clear()
{
/*
     * Finally when the loop is left smoothly - asked by user in the command line interface - we unregister our client from it
     */
#ifdef WITH_TINYDTLS
  free(pskBuffer);
#endif

#ifdef LWM2M_BOOTSTRAP
  close_backup_object();
#endif
  lwm2m_close(lwm2mH);

  close(data.sock);
  connection_free(data.connList);

  clean_security_object(objArray[0]);
  lwm2m_free(objArray[0]);
  clean_server_object(objArray[1]);
  lwm2m_free(objArray[1]);
  free_object_device(objArray[2]);
  free_object_firmware(objArray[3]);
  free_object_location(objArray[4]);
  free_object_conn_m(objArray[6]);
  free_object_conn_s(objArray[7]);
  acl_ctrl_free_object(objArray[8]);

#ifdef MEMORY_TRACE
  if (g_quit == 1)
  {
    trace_print(0, 1);
  }
#endif
}

#ifndef PAYLOADDEF
#define PAYLOADDEF

struct ClientsListDescription {
  uint32_t num_active_ids;
  uint32_t own_id;
  uint32_t client_ids[1000]; // maximum of 1000 clients could be send within one
                             // message
};

struct MessageAllDescription {
  uint32_t number_of_elements;
  char message[2048];
  uint32_t sender_id;
};

struct NotifyOneDescription {
  uint32_t sender_id;
  uint32_t receiver_id;
  char message[2048];
};

#endif /* PAYLOADDEF */
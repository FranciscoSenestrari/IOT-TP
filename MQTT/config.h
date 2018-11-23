typedef struct {
  // Accesspoint 
  char essid[50];
  char pass[50];

  // Broker
  char broker_add[50];
  int broker_puerto;
  char broker_topic[100];
  char clientID[10];

  //extras
  char admin_password[20];
  char has_batt;  
} config_t;

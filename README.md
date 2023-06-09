Implementation of SOCKS: 
A protocol for TCP proxy across firewalls.
This protocol relays TCP sessions at a firewall host to allow application
users transparent access across the firewall.

ver3_procy.c - The SOCKS4 proxy server that forwards client requests.
new_client.c - The client that sends request to the application server through the proxy server.
validation_new.h - Validates the client request from the proxy server.
package main

import (
	pfdeclient "client_go/client"
)

func main() {
	var client pfdeclient.TFDEClient
	client.ListenAndServe("localhost:8002", nil)
}

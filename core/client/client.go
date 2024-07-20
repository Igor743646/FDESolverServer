package main

import (
	pfdeclient "client/client_impl"
	"log"
)

func main() {
	var client pfdeclient.TFDEClient
	err := client.ListenAndServe("localhost:8002", nil)
	if err != nil {
		log.Panicln(err.Error())
	}
}

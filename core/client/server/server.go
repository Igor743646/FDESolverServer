package main

import (
	"log"
	pfdeclient "server/server_impl"
)

func main() {
	var client pfdeclient.TFDEClient
	err := client.ListenAndServe("localhost:8002", nil)
	if err != nil {
		log.Panicln(err.Error())
	}
}

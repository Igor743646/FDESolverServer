package pfdeclient

import (
	"client/router"
	"log"
	"net/http"

	"github.com/gorilla/mux"
)

var (
	gTemplatesPath = "static/templates"
	gImagesPath    = "static/images"
	gScriptsPath   = "static/scripts"
)

type TFDEClient struct {
}

func NewTFDEClient() *TFDEClient {
	return &TFDEClient{}
}

func (cl *TFDEClient) ListenAndServe(addr string, handler http.Handler) error {

	homeRouter := mux.NewRouter()
	homeRouter.HandleFunc("/home", router.Home).Methods(http.MethodGet)
	homeRouter.HandleFunc("/home", router.RunTask).Methods(http.MethodPost)

	http.Handle("/home", homeRouter)
	http.Handle("/templates/", http.StripPrefix("/templates/", http.FileServer(http.Dir(gTemplatesPath))))
	http.Handle("/images/", http.StripPrefix("/images/", http.FileServer(http.Dir(gImagesPath))))
	http.Handle("/scripts/", http.StripPrefix("/scripts/", http.FileServer(http.Dir(gScriptsPath))))
	http.Handle("/", http.RedirectHandler("/home", http.StatusMovedPermanently))

	log.Printf("Server start on: %s", addr)
	return http.ListenAndServe(addr, handler)
}

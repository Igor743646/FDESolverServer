package pfdeclient

import (
	"client_go/router"
	"net/http"

	"github.com/gorilla/mux"
)

var (
	gTemplatesPath = "static/templates"
)

type TFDEClient struct {
}

func NewTFDEClient() *TFDEClient {
	return &TFDEClient{}
}

func (cl *TFDEClient) ListenAndServe(addr string, handler http.Handler) error {
	fs := http.FileServer(http.Dir(gTemplatesPath))

	homeRouter := mux.NewRouter()
	homeRouter.HandleFunc("/home", router.Home).Methods(http.MethodGet)
	homeRouter.HandleFunc("/home", router.RunTask).Methods(http.MethodPost)

	http.Handle("/home", homeRouter)
	http.Handle("/templates/", http.StripPrefix("/templates/", fs))
	http.Handle("/", http.RedirectHandler("/home", http.StatusMovedPermanently))
	return http.ListenAndServe(addr, handler)
}

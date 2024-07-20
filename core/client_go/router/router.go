package router

import (
	"client_go/drawer"
	pb "client_go/protos/generated"
	"context"
	"html/template"
	"log"
	"net/http"
	"strings"

	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
	"google.golang.org/protobuf/encoding/protojson"
)

type TIndexTemplateData struct {
	Error    error
	Config   string
	WorkTime float64
}

var (
	gHtmlFiles = []string{
		"./static/templates/index.html",
		"./static/templates/base.html",
		"./static/templates/config_example.html",
	}
)

func Home(writer http.ResponseWriter, request *http.Request) {
	indexTemplate := template.Must(template.ParseFiles(gHtmlFiles...))
	err := indexTemplate.Execute(writer, nil)

	if err != nil {
		log.Println(err.Error())
		http.Error(writer, "Internal Server Error", 500)
		return
	}
}

func ReplyErrorPage(writer http.ResponseWriter, result TIndexTemplateData) {
	indexTemplate := template.Must(template.ParseFiles(gHtmlFiles...))

	log.Println("Error: ", result.Error.Error())
	err := indexTemplate.Execute(writer, result)

	if err != nil {
		log.Println(err.Error())
		http.Error(writer, "Internal Server Error", 500)
		return
	}
}

func ParseProtoConfig(request *http.Request, result *TIndexTemplateData) (*pb.TClientConfig, error) {
	result.Config = strings.TrimSpace(request.FormValue("config"))
	log.Println(result.Config)

	var pbConfig pb.TClientConfig
	err := protojson.Unmarshal([]byte(result.Config), &pbConfig)

	if err != nil {
		return nil, err
	}

	return &pbConfig, nil
}

func RunTask(writer http.ResponseWriter, request *http.Request) {
	var result TIndexTemplateData

	pbConfig, err := ParseProtoConfig(request, &result)

	if err != nil {
		result.Error = err
		ReplyErrorPage(writer, result)
		return
	}

	log.Println(&pbConfig)

	var dialOpts []grpc.DialOption

	dialOpts = append(dialOpts, grpc.WithTransportCredentials(insecure.NewCredentials()))

	conn, err := grpc.NewClient("[::]:50051", dialOpts...)
	if err != nil {
		result.Error = err
		ReplyErrorPage(writer, result)
		return
	}
	defer conn.Close()

	client := pb.NewTFDESolverServerClient(conn) // pb.NewRouteGuideClient(conn)
	pbResults, err := client.RunTask(context.Background(), pbConfig)
	if err != nil {
		result.Error = err
		ReplyErrorPage(writer, result)
		return
	}

	indexTemplate := template.Must(template.ParseFiles(gHtmlFiles...))
	err = indexTemplate.Execute(writer, result)
	if err != nil {
		result.Error = err
		ReplyErrorPage(writer, result)
		return
	}

	resultDrawer := drawer.NewTResultDrawer(pbResults)
	resultDrawer.Render(writer)
}

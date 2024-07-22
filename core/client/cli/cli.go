package main

// import (
// 	"log"

// 	"github.com/Igor743646/FDESolverServer/core/client/drawer"
// )

// func main() {
// 	resultDrawer := drawer.NewTResultDrawer()

// 	log.Println(resultDrawer)
// }
import (
	"log"
	"os"

	"github.com/Igor743646/FDESolverServer/core/client/drawer"
	pb "github.com/Igor743646/FDESolverServer/core/client/protos"
	"github.com/urfave/cli"
	"google.golang.org/protobuf/encoding/protojson"
	"google.golang.org/protobuf/proto"
)

func main() {
	app := cli.NewApp()
	app.Name = "FDESolver Result Drawer CLI"
	app.Usage = "Take result file of FDESolver CLI and draw html/png data"
	app.Version = "1.0.0"

	// We'll be using the same flag for all our commands
	// so we'll define it up here
	myFlags := []cli.Flag{
		cli.StringFlag{
			Name:  "input-file",
			Usage: "Path to file with serialized TResults",
			Value: "result.txt",
		},
		cli.StringFlag{
			Name:  "output-path",
			Usage: "Path for drawing result",
			Value: "result",
		},
		cli.BoolFlag{
			Name:  "input-json",
			Usage: "Is json serialized",
		},
	}

	// we create our commands
	app.Commands = []cli.Command{
		{
			Name:  "html",
			Usage: "Draw HTML result",
			Flags: myFlags,
			// the action, or code that will be executed when
			// we execute our `ns` command
			Action: func(c *cli.Context) error {
				inputFile := c.String("input-file")
				isJson := c.Bool("input-json")

				data, err := os.ReadFile(inputFile)
				if err != nil {
					return err
				}

				var results pb.TResults
				if isJson {
					err = protojson.Unmarshal(data, &results)
				} else {
					err = proto.Unmarshal(data, &results)
				}

				if err != nil {
					return err
				}

				ouputFileName := c.String("output-path") + ".html"
				ouputFile, err := os.OpenFile(ouputFileName, os.O_RDWR|os.O_CREATE, 0644)
				if err != nil {
					return err
				}

				resultDrawer := drawer.NewTResultDrawer(&results)
				resultDrawer.Render(ouputFile)

				return nil
			},
		},
	}

	// start our application
	err := app.Run(os.Args)
	if err != nil {
		log.Fatal(err)
	}
}

package drawer

import (
	"fmt"
	"math"
	"net/http"
	"slices"

	pb "github.com/Igor743646/FDESolverServer/core/client/protos"

	"github.com/go-echarts/go-echarts/v2/charts"
	"github.com/go-echarts/go-echarts/v2/components"
	"github.com/go-echarts/go-echarts/v2/opts"
	"github.com/montanaflynn/stats"
)

type TResultDrawer struct {
	Results *pb.TResults
	Config  *pb.TSolverConfig
}

func NewTResultDrawer(pbResults *pb.TResults) TResultDrawer {
	return TResultDrawer{Results: pbResults, Config: pbResults.GetTask()}
}

func (rd *TResultDrawer) Render(writer http.ResponseWriter) {
	for _, result := range rd.Results.Results {
		rd.RenderResult(writer, result)
	}
}

func (rd *TResultDrawer) RenderResult(writer http.ResponseWriter, result *pb.TResult) {
	writer.Write([]byte(fmt.Sprintf(
		`<div class="body-block" style="font-size:x-large;text-align:center">%s</div>`, result.GetMethodName(),
	)))

	writer.Write([]byte(`<div class="result-block">`))

	page := components.NewPage()
	page.SetLayout(components.PageFlexLayout)
	page.SetPageTitle(result.GetMethodName())
	page.AddCharts(
		rd.DrawHeatMap(result),
		rd.DrawSurface(result),
	)
	if rd.Results.GetRealSolution() != nil {
		page.AddCharts(
			rd.DrawRealSolSurface(),
			rd.DrawErrorSurface(result),
			rd.DrawErrorBoxplot(result),
		)
	}
	page.Render(writer)

	writer.Write([]byte(`</div>`))
}

func (rd *TResultDrawer) DrawHeatMap(result *pb.TResult) *charts.HeatMap {
	hm := charts.NewHeatMap()
	hm.SetGlobalOptions(
		rd.GetInitializationOpts(),
		rd.GetTitleOpts("Heat Map"),
		charts.WithXAxisOpts(opts.XAxis{
			Type:        "category",
			Name:        "x",
			SplitNumber: 10,
			SplitArea:   &opts.SplitArea{Show: opts.Bool(true)},
		}),
		charts.WithYAxisOpts(opts.YAxis{
			Type:        "value",
			Name:        "t",
			SplitNumber: 10,
			SplitArea:   &opts.SplitArea{Show: opts.Bool(true)},
		}),
		charts.WithVisualMapOpts(opts.VisualMap{
			Calculable: opts.Bool(true),
			Min:        float32(slices.Min(result.Field.Data)),
			Max:        float32(slices.Max(result.Field.Data)),
			InRange: &opts.VisualMapInRange{
				Color: []string{"#50a3ba", "#eac736", "#d94e5d"},
			},
		}),
	)

	hm.AddSeries("", rd.GenHeatMapData(result))
	return hm
}

func (rd *TResultDrawer) DrawSurface(result *pb.TResult) *charts.Surface3D {
	sur := charts.NewSurface3D()
	sur.SetGlobalOptions(
		rd.GetInitializationOpts(),
		rd.GetTitleOpts("Surface 3D of calculated result"),
		charts.WithZAxis3DOpts(opts.ZAxis3D{
			Type: "value",
			Name: "U(x, t)",
			Min:  float32(slices.Min(result.Field.Data)),
			Max:  float32(slices.Max(result.Field.Data)),
		}),
		charts.WithXAxis3DOpts(opts.XAxis3D{
			Type: "value",
			Name: "x",
			Min:  rd.Config.LeftBound,
			Max:  rd.Config.RightBound,
		}),
		charts.WithYAxis3DOpts(opts.YAxis3D{
			Type: "value",
			Name: "t",
			Min:  0,
			Max:  rd.Config.MaxTime,
		}),
		charts.WithVisualMapOpts(opts.VisualMap{
			Calculable: opts.Bool(true),
			Min:        float32(slices.Min(result.Field.Data)),
			Max:        float32(slices.Max(result.Field.Data)),
			InRange: &opts.VisualMapInRange{
				Color: []string{"#313695", "#4575b4", "#74add1", "#abd9e9", "#e0f3f8"},
			},
		}),
	)

	sur.AddSeries("", rd.GenSurfaceData(result.Field))
	return sur
}

func (rd *TResultDrawer) DrawRealSolSurface() *charts.Surface3D {
	sur := charts.NewSurface3D()
	sur.SetGlobalOptions(
		rd.GetInitializationOpts(),
		rd.GetTitleOpts("Surface 3D of real solution"),
		charts.WithZAxis3DOpts(opts.ZAxis3D{
			Type: "value",
			Name: *rd.Results.RealSolutionName,
		}),
		charts.WithXAxis3DOpts(opts.XAxis3D{
			Type: "value",
			Name: "x",
			Min:  rd.Config.LeftBound,
			Max:  rd.Config.RightBound,
		}),
		charts.WithYAxis3DOpts(opts.YAxis3D{
			Type: "value",
			Name: "t",
			Min:  0,
			Max:  rd.Config.MaxTime,
		}),
		charts.WithVisualMapOpts(opts.VisualMap{
			Calculable: opts.Bool(true),
			Min:        float32(slices.Min(rd.Results.RealSolution.Data)),
			Max:        float32(slices.Max(rd.Results.RealSolution.Data)),
			InRange: &opts.VisualMapInRange{
				Color: []string{"#313695", "#4575b4", "#74add1", "#abd9e9", "#e0f3f8"},
			},
		}),
	)

	sur.AddSeries("", rd.GenSurfaceData(rd.Results.RealSolution))
	return sur
}

func (rd *TResultDrawer) DrawErrorSurface(result *pb.TResult) *charts.Surface3D {
	errsd, min, max := rd.GenErrorSurfaceData(result.Field)
	sur := charts.NewSurface3D()
	sur.SetGlobalOptions(
		rd.GetInitializationOpts(),
		rd.GetTitleOpts("Error Surface 3D of real solution"),
		charts.WithZAxis3DOpts(opts.ZAxis3D{
			Type: "value",
			Name: "|u(x, t) - U(x, t)|",
		}),
		charts.WithXAxis3DOpts(opts.XAxis3D{
			Type: "value",
			Name: "x",
			Min:  rd.Config.LeftBound,
			Max:  rd.Config.RightBound,
		}),
		charts.WithYAxis3DOpts(opts.YAxis3D{
			Type: "value",
			Name: "t",
			Min:  0,
			Max:  rd.Config.MaxTime,
		}),
		charts.WithVisualMapOpts(opts.VisualMap{
			Calculable: opts.Bool(true),
			Min:        float32(min),
			Max:        float32(max) / 2,
			InRange: &opts.VisualMapInRange{
				Color: []string{"#68fa99", "#708f00", "#ff2400"},
			},
		}),
	)

	sur.AddSeries("", errsd)
	return sur
}

func (rd *TResultDrawer) DrawErrorBoxplot(result *pb.TResult) *charts.BoxPlot {

	bp := charts.NewBoxPlot()
	bp.SetGlobalOptions(
		charts.WithInitializationOpts(opts.Initialization{
			Width:  "750px",
			Height: "500px",
		}),
		rd.GetTitleOpts("Error box plots"),
		charts.WithXAxisOpts(opts.XAxis{
			Type: "category",
			Name: "x",
		}),
	)

	bp.AddSeries("", rd.GenErrorBoxplotData(result))
	return bp
}

func (rd *TResultDrawer) GenErrorBoxplotData(result *pb.TResult) []opts.BoxPlotData {
	items := make([]opts.BoxPlotData, result.Field.Rows)
	errorData := make([]float64, result.Field.Columns)
	for i := uint64(0); i < result.Field.Rows; i++ {
		for j := uint64(0); j < result.Field.Columns; j++ {
			idx := i*result.Field.Columns + j
			errorData[j] = math.Abs(rd.Results.RealSolution.Data[idx] - result.Field.Data[idx])
		}

		min := slices.Min(errorData)
		max := slices.Max(errorData)
		q, _ := stats.Quartile(errorData)

		items[i] = opts.BoxPlotData{
			Name: fmt.Sprintf("Point\n t:%f", float64(i)*rd.Config.TimeStep),
			Value: []float64{
				min,
				q.Q1,
				q.Q2,
				q.Q3,
				max,
			},
		}
	}
	return items
}

func (rd *TResultDrawer) GetInitializationOpts() charts.GlobalOpts {
	return charts.WithInitializationOpts(opts.Initialization{
		Width:  "550px",
		Height: "500px",
	})
}

func (rd *TResultDrawer) GetTitleOpts(title string) charts.GlobalOpts {
	return charts.WithTitleOpts(opts.Title{
		Show:  opts.Bool(true),
		Title: title,
	})
}

func (rd *TResultDrawer) GenHeatMapData(result *pb.TResult) []opts.HeatMapData {
	items := make([]opts.HeatMapData, len(result.Field.Data))
	for i := uint64(0); i < result.Field.Rows; i++ {
		for j := uint64(0); j < result.Field.Columns; j++ {
			idx := i*result.Field.Columns + j
			items[idx] = opts.HeatMapData{
				Name:  fmt.Sprintf("Point\nx: %f t:%f", rd.Config.LeftBound+float64(j)*rd.Config.SpaceStep, float64(i)*rd.Config.TimeStep),
				Value: [3]interface{}{ /*x=*/ j /*t=*/, i, result.Field.Data[idx]},
			}
		}
	}
	return items
}

func (rd *TResultDrawer) GenSurfaceData(resultField *pb.TMatrix) []opts.Chart3DData {
	items := make([]opts.Chart3DData, len(resultField.Data))
	for i := uint64(0); i < resultField.Rows; i++ {
		for j := uint64(0); j < resultField.Columns; j++ {
			idx := i*resultField.Columns + j
			items[idx] = opts.Chart3DData{
				Name:  fmt.Sprintf("Point\nx: %f t:%f", rd.Config.LeftBound+float64(j)*rd.Config.SpaceStep, float64(i)*rd.Config.TimeStep),
				Value: []interface{}{rd.Config.LeftBound + float64(j)*rd.Config.SpaceStep, float64(i) * rd.Config.TimeStep, resultField.Data[idx]},
			}
		}
	}
	return items
}

func (rd *TResultDrawer) GenErrorSurfaceData(resultField *pb.TMatrix) (items []opts.Chart3DData, min float64, max float64) {
	min = math.Abs(resultField.Data[0] - rd.Results.RealSolution.Data[0])
	max = min
	items = make([]opts.Chart3DData, len(resultField.Data))
	for i := uint64(0); i < resultField.Rows; i++ {
		for j := uint64(0); j < resultField.Columns; j++ {
			idx := i*resultField.Columns + j
			x := rd.Config.LeftBound + float64(j)*rd.Config.SpaceStep
			y := float64(i) * rd.Config.TimeStep
			z := math.Abs(resultField.Data[idx] - rd.Results.RealSolution.Data[idx])
			items[idx] = opts.Chart3DData{
				Name:  fmt.Sprintf("Point\nx: %f t:%f", x, y),
				Value: []interface{}{x, y, z},
			}

			if z < min {
				min = z
			}
			if z > max {
				max = z
			}
		}
	}
	return items, min, max
}

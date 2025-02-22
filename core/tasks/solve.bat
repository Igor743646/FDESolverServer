
cd "C:\Igor\Projects\c++\FDESolverServer\core\server\bin\Release"
.\FDESolverCLI_Release.exe --input-file "C:\Igor\Projects\c++\FDESolverServer\core\tasks\task4.txt" --output-file "C:\Igor\Projects\c++\FDESolverServer\core\tasks\results\result.txt"

cd "C:\Igor\Projects\c++\FDESolverServer\core\client\cli\"
go run cli.go html --input-file "C:\Igor\Projects\c++\FDESolverServer\core\tasks\results\result.txt" --output-path C:\Igor\Projects\c++\FDESolverServer\core\tasks\results\result

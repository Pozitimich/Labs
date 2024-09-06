#load "Eval_apply.fsx"


System.Console.WriteLine "Enter file path"

let filename = System.Console.ReadLine()
let expr = Parser.to_parce filename
let b = Interpreter.eval expr Map.empty
System.Console.WriteLine b
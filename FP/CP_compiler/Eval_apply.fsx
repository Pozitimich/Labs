module Interpreter
#load "Parser.fsx"

let funof = function
| "+" -> 
   function
   | [Types.Int(a);Types.Int(b)] -> Types.Int(a+b)
   | [Types.Float(a);Types.Float(b)] -> Types.Float(a+b)
   | [Types.Float(a);Types.Int(b)] -> Types.Float(a+float(b))
   | [Types.Int(a);Types.Float(b)] -> Types.Float(float(a)+b)
   | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "-" -> 
   function
      | [Types.Int(a);Types.Int(b)] -> Types.Int(a-b)
      | [Types.Float(a);Types.Float(b)] -> Types.Float(a-b)
      | [Types.Float(a);Types.Int(b)]  -> Types.Float(a-float(b)) 
      | [Types.Int(a);Types.Float(b)] -> Types.Float(float(a)-b)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "*" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> Types.Int(a*b)
      | [Types.Float(a);Types.Float(b)]  -> Types.Float(a*b)
      | [Types.Float(a);Types.Int(b)] -> Types.Float(a*float(b))
      | [Types.Int(a);Types.Float(b)] -> Types.Float(float(a)*b)
      | t -> printfn "Type error"; failwith "Wrong types used in operation"
| "%" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> Types.Int(a%b)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "/" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> Types.Int(a/b)
      | [Types.Float(a);Types.Float(b)]  -> Types.Float(a/b)
      | [Types.Float(a);Types.Int(b)]  -> Types.Float(a/float(b)) 
      | [Types.Int(a);Types.Float(b)]  -> Types.Float(float(a)/b)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "**" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> Types.Float(float(a) ** float(b))
      | [Types.Float(a);Types.Float(b)]  -> Types.Float(a ** b)
      | [Types.Float(a);Types.Int(b)]  -> Types.Float(a ** float(b))
      | [Types.Int(a);Types.Float(b)]  -> Types.Float(float(a) ** b)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "=" -> 
   function
      | [Types.Int(a);Types.Int(b)] ->  if a=b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Float(b)] -> if a=b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Int(b)] -> if a=float(b) then Types.Int(1) else Types.Int(0)
      | [Types.Int(a);Types.Float(b)] -> if float(a)=b then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| ">" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> if a>b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Float(b)]  -> if a>b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Int(b)]  -> if a>float(b) then Types.Int(1) else Types.Int(0) 
      | [Types.Int(a);Types.Float(b)]  -> if float(a)>b then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "<" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> if a<b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Float(b)]  -> if a<b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Int(b)]  -> if a<float(b) then Types.Int(1) else Types.Int(0)
      | [Types.Int(a);Types.Float(b)]  ->  if float(a)<b then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "<=" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> if a<=b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Float(b)]  -> if a<=b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Int(b)]  -> if a<=float(b) then Types.Int(1) else Types.Int(0) 
      | [Types.Int(a);Types.Float(b)]  -> if float(a)<=b then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| ">=" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> if a>=b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Float(b)]  -> if a>=b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Int(b)]  -> if a>=float(b) then Types.Int(1) else Types.Int(0) 
      | [Types.Int(a);Types.Float(b)]  -> if float(a)>=b then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "!=" -> 
   function
      | [Types.Int(a);Types.Int(b)]  -> if a<>b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Float(b)] -> if a<>b then Types.Int(1) else Types.Int(0)
      | [Types.Float(a);Types.Int(b)]  -> if a<>float(b) then Types.Int(1) else Types.Int(0)
      | [Types.Int(a);Types.Float(b)] -> if float(a)<>b then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "||" ->
   function
      | [Types.Int(a);Types.Int(b)] -> if a + b > 0 then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "&&" ->
   function
      | [Types.Int(a);Types.Int(b)]  -> if a * b > 0 then Types.Int(1) else Types.Int(0)
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| "sqrt" ->
   function
      | [Types.Float(a)] -> Types.Float(sqrt a)
      | [Types.Int(a)] -> Types.Float(sqrt (float a))
      | _ -> printfn "Type error"; failwith "Wrong types used in operation"
| _ -> printfn "Operation error"; failwith "Operation does not exist"

let rec eval exp env =
  match exp with
  | Types.Int(n) -> Types.Int(n)
  | Types.Float(n) -> Types.Float(n)
  | Types.Bool(n) -> Types.Bool(n)
  | Types.Void -> Types.Void
  | Types.Var(x) -> Map.find x env
  | Types.Lam(id,ex) -> Types.Closure(exp,env)
  | Types.App(ex1,ex2) -> apply (eval ex1 env) (eval ex2 env)
  | Types.PFunc(id) -> Types.Op(id,(Parser.unarity id),[])
  | Types.Cond(e0,e1,e2) ->
     if eval e0 env=Types.Int(0) then eval e2 env else eval e1 env
  | Types.Let(id,e1,e2) ->
     let e1' = eval e1 env in eval e2 (Map.add id e1' env)
  | Types.LetRec(id,e1,e2) ->
      let e1' = Types.RClosure(e1,env,id) in eval e2 (Map.add id e1' env) 
  | _ -> printfn "Error occured"; failwith "syntax error"
and apply e1 e2 =
  printfn "%A\n%A" e1 e2
  if(e2 = Types.Void)
  then e1
  else
      match e1 with
      | Types.Closure(Types.Lam(v,e),env) -> eval e (Map.add v e2 env)
      | Types.RClosure(Types.Lam(v,e),env, id) -> eval e (Map.add v e2 (Map.add id e1 env))
      | Types.Op(id,n,args) ->
          if n=1 then (funof id)(e2::args)
          else Types.Op(id,n-1,e2::args)
      | Types.Void -> e2
      |_ -> printfn "Error occured"; failwith "syntax error"

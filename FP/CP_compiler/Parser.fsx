module Parser
#load "Types.fsx"

open System
open System.IO

let priority =
    function
    | "**" -> 1
    | "sqrt" -> 1
    | "*" -> 2
    | "/" -> 2
    | "%" -> 2
    | "+" -> 3
    | "-" -> 3
    | ">" -> 4
    | ">=" -> 4
    | "<" -> 4
    | "<=" -> 4
    | "=" -> 5
    | "!=" -> 5
    | "&&" -> 6
    | "||" -> 7
    | _ -> 0

let unarity id =
   let un_ops = ["sqrt"]
   let bin_ops = ["+";"-";"*" ;"/";"%";"**";"=";">";"<";">=";"<=";"!=";"||";"&&"]
   if List.contains id bin_ops then 2
   elif List.contains id un_ops then 1
   else printfn "Operation error"; failwith "Operation does not exist"

let super_read (file:string) =
    use stream = new StreamReader (file)
    let rec super_read' (buf:string) list read =
        if stream.EndOfStream then list
        else
            let c = (char)(stream.Read())
            if c = '#' then 
                if buf.Length <> 0 then 
                    super_read' "" (list @ [buf]) (not(read)) 
                else super_read' "" list (not(read))
            elif not(read) || c = ' ' || c = '\t' || c = '\n' || c = '\r' then
                if buf.Length <> 0 then
                    super_read' "" (list @ [buf]) read 
                else super_read' buf list read 
            elif c = '{' || c = '}' || c = '[' || c = ']' || c = '(' || c = ')' then
                if buf.Length <> 0 then
                    super_read' "" (list @ [buf] @ [c.ToString()]) read
                else super_read' "" (list @ [c.ToString()]) read
            elif c = '$' then
                if buf.Length <> 0 then
                    super_read' "$" (list @ [buf]) read
                else super_read' "$" list read
            else
                super_read' (buf + c.ToString()) list read
    super_read' "" [] true

let rec args_group list =
    let rec args_group' list lst result layer read = 
        if(not(List.isEmpty list)) then
            let head = List.head list
            let list = List.tail list
            if (head = "[" && read) then
                    let layer = layer + 1
                    args_group' list lst (result @ [head]) layer read
            elif (head = "]" && read) then
                    let layer = layer - 1
                    if (layer = 0) then
                        args_group' list lst result layer false
                    else
                        args_group' list lst (result @ [head]) layer read
            else
                if (layer = 0) then
                    args_group' list (lst @ [head]) result layer false
                else
                    args_group' list lst (result @ [head]) layer read
        else
            result @ ["]"], lst
    args_group' list [] [] 0 true

let rec group_vars list acc =
    let new_element, new_list =
        let (head:string) = List.head list
        if list.Length > 1 && head.StartsWith("$") && list[1].StartsWith("[") then
            let result, lst = args_group (List.tail list) 
            [head] @ result, lst
        else 
            [List.head list], List.tail list
    if(List.isEmpty new_list) then
        acc @ [new_element]
    else
        group_vars new_list (acc @ [new_element])

let rec consider_brackets x y =
    let x_tail = List.tail x
    if List.head x <> ["("] then
        consider_brackets x_tail ([List.head x] @ y)
    else
        (x_tail,y)

let rec infix_constr op_stack symb_stack =
    if not(List.isEmpty op_stack) then
        let h = List.head op_stack
        infix_constr (List.tail op_stack) ([h] @ symb_stack)
    else
        List.concat symb_stack

let rec prior_distr op_stack symb_stack op_head head=
    let ar = priority (List.head head)
    if ( not(List.isEmpty op_stack) && op_head <= ar && List.head(List.head op_stack) <> "(") then
            let head = List.head op_stack
            let op_stack = List.tail op_stack
            let symb_stack = [head] @ symb_stack
            if ( not(List.isEmpty op_stack)) then 
                prior_distr op_stack symb_stack (priority ( List.head(List.head op_stack) )) head
            else
                prior_distr op_stack symb_stack op_head head
    else
        (op_stack, symb_stack, op_head)

let infix_to_prefix list =
    let rec infix_to_prefix' list op_stack symb_stack op_head new_list = 
        if not (List.isEmpty (new_list)) then
          let head = List.head new_list
          let tail = List.tail new_list
      
          if(List.head head = "(") then
            infix_to_prefix' (list) ([head] @ op_stack) symb_stack op_head tail
          elif(List.head head = ")") then
            let op_stack,symb_stack = consider_brackets op_stack symb_stack
            infix_to_prefix' list op_stack symb_stack op_head tail
          elif(priority (List.head head) = 0) then
            infix_to_prefix' list op_stack ([head] @ symb_stack) op_head tail
          elif((List.isEmpty op_stack) || List.head (List.head op_stack) = ")") then
            infix_to_prefix' list ([head] @ op_stack) symb_stack op_head tail
          else
            let op_head = priority (List.head(List.head op_stack))
            if(op_head > priority (List.head head)) then
              infix_to_prefix' list ([head] @ op_stack) symb_stack op_head tail
            else
              let op_stack, symb_stack, op_head = prior_distr op_stack symb_stack op_head head
              infix_to_prefix' list ([head] @ op_stack) symb_stack op_head tail
        else
            infix_constr op_stack symb_stack
    infix_to_prefix' list [] [] 0 (group_vars list [])


let rec make_tab lst (s:string) (symbols:list<string>) =
    if s.Length = 1 || (List.isEmpty symbols) then
        lst @ [s]
    else
        let symb = List.head symbols
        if s.StartsWith(symb) && s.Length <> 0
            then lst @ [symb] @ [s[1..]]
        else make_tab lst s (List.tail symbols)

let to_prefix list =
    let rec to_prefix' list result lst in_expr layer =
        if (not (List.isEmpty list) ) then
            let head = List.head list
            let list = List.tail list
            if(not(in_expr) && head = "{") then
                to_prefix' list result lst true layer
            elif not(in_expr) then
                to_prefix' list (result @ [head]) lst in_expr layer
            elif(in_expr && head = "{") then
                to_prefix' list result (lst @ [head]) in_expr (layer+1)
            elif(in_expr && head = "}" && layer = 0) then
                let x = to_prefix' (infix_to_prefix lst) [] [] false 0
                to_prefix' list (result @ x) [] false layer
            elif(in_expr && head = "}") then
                to_prefix' list result (lst @ [head]) in_expr (layer-1)
            else
                to_prefix' list result (lst @ [head]) in_expr layer
        else
            result
    to_prefix' list [] [] false 0

let parce list =
    let mutable lst = list
    let rec parse' (_: Types.expr) =
        if List.isEmpty(lst) then Types.Void
        else
        let head = List.head lst
        match head with      
        | "let" -> 
            let name = lst[1]
            lst <- lst[2..]
            let (expr1) = parse' Types.Void
            let (expr2) = parse' Types.Void
            Types.Let(name, expr1, expr2)
        | "let_rec" ->
            let name = lst[1]
            lst <- lst[2..]
            let (expr1) = parse' Types.Void
            let (expr2) = parse' Types.Void
            Types.LetRec(name, expr1, expr2)
        | "Lam" ->
            let name = lst[1]
            lst <- lst[2..]
            let (expr1) = parse' Types.Void
            Types.Lam(name, expr1)
        | s when s.StartsWith("$") -> 
            let name = s[1..]
            if(List.tail lst <> [] && lst[1] = "[") then
                lst <- lst[2..]
                parce_args (Types.Var(name))
            else 
                lst <- List.tail lst
                Types.Var(name)

        | temp when priority temp <> 0 ->
            let op = head
            let unar = unarity op
            lst <- List.tail lst
            if unar = 1 then
                let (expr1) = parse' Types.Void
                Types.App(Types.PFunc(op), expr1)
            elif unar = 2 then
                let (expr1) = parse' Types.Void
                let (expr2) = parse' Types.Void
                Types.App(Types.App(Types.PFunc(op), expr1), expr2)
            else 
                printfn "Cannot define the unarity of operation"
                failwith "internal error"
        
        | "if" ->
            lst <- List.tail lst
            let (expr1) = parse' Types.Void
            let (expr2) = parse' Types.Void
            if List.head lst = "else" then
                lst <- List.tail lst
                let (expr3) = parse' Types.Void
                Types.Cond(expr1, expr2, expr3)
            else 
                let (expr3) = parse' Types.Void
                Types.Cond(expr1, expr2, expr3)
        | "void" ->
            lst <- List.tail lst
            Types.Void

        | _ ->
            let is_int, int_num = Int32.TryParse (head)
            let is_float, fl_num = Double.TryParse (head)
            if is_int then
                lst <- List.tail lst
                Types.Int(int_num)
            elif is_float then
                lst <- List.tail lst
                Types.Float(fl_num)
            else
                printfn "%s" (List.head lst); failwith "syntax error"
    and parce_args (expr: Types.expr) =
        if (List.head lst) = "]" then
            lst <- List.tail lst
            expr
        else
            let (expr1) = parse' Types.Void
            parce_args (Types.App(expr, expr1))
    parse' Types.Void

let to_parce file =
   printfn "%A" (file |> super_read |> to_prefix)
   file 
    |> super_read
    |> to_prefix
    |> parce
module Types

type id = string

type expr =
| App of expr*expr
| Lam of id*expr
| Var of id
| Void
| Int of int
| Float of float
| PFunc of id
| Cond of expr*expr*expr
| Let of id*expr*expr
| LetRec of id*expr*expr
| Op of id*int*expr list
| Closure of expr*env
| RClosure of expr*env*id
and
  env = Map<id,expr>


 
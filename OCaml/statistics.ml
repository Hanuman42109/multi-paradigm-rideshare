(*
  statistics.ml

  Functional implementation of basic descriptive statistics
  (mean, median, mode) on a list of integers.

  Paradigm: Functional (OCaml)
  - Data is represented as an immutable int list throughout.
  - Each statistic is derived with higher-order functions
    (List.fold_left, List.map, List.filter, List.sort) rather than
    explicit loops or mutable accumulators.
  - No ref cells, no array mutation: every intermediate value is a
    brand new immutable list/tuple built from the previous one.

  Build:  ocamlfind ocamlopt -package str -linkpkg statistics.ml -o statistics
          (or simply: ocaml statistics.ml  to run via the toplevel)
  Run:    ./statistics
*)

(* ---------- Mean ----------
   fold_left accumulates the running sum without any mutable
   variable; the list is consumed functionally from the left. *)
let mean (data : int list) : float =
  let sum = List.fold_left ( + ) 0 data in
  let n = List.length data in
  float_of_int sum /. float_of_int n

(* ---------- Median ----------
   List.sort returns a new, sorted list -- the original `data`
   binding is never touched, preserving immutability. *)
let median (data : int list) : float =
  let sorted = List.sort compare data in
  let n = List.length sorted in
  if n mod 2 = 1 then
    float_of_int (List.nth sorted (n / 2))
  else
    let mid1 = List.nth sorted (n / 2 - 1) in
    let mid2 = List.nth sorted (n / 2) in
    float_of_int (mid1 + mid2) /. 2.0

(* ---------- Mode ----------
   Strategy, entirely without mutable state:
     1. Sort the list so equal values are adjacent.
     2. Fold over the sorted list, building an immutable
        (value, count) association list -- each step produces a
        brand new list rather than mutating an existing one.
     3. Use List.fold_left again to find the maximum count.
     4. Use List.filter + List.map to pull out every value that
        reaches that maximum count (handles ties / multi-modal data).
*)
let frequencies (data : int list) : (int * int) list =
  let sorted = List.sort compare data in
  List.fold_left
    (fun acc x ->
       match acc with
       | (v, c) :: rest when v = x -> (v, c + 1) :: rest
       | _ -> (x, 1) :: acc)
    []
    sorted

let mode (data : int list) : (int list * int) =
  let freqs = frequencies data in
  let highest =
    List.fold_left (fun best (_, c) -> max best c) 0 freqs
  in
  let modes =
    freqs
    |> List.filter (fun (_, c) -> c = highest)
    |> List.map fst
    |> List.sort compare
  in
  (modes, highest)

(* ---------- Utility: render an int list as "[a, b, c]" ---------- *)
let string_of_int_list (xs : int list) : string =
  "[" ^ String.concat ", " (List.map string_of_int xs) ^ "]"

(* ---------- Demo driver ---------- *)
let () =
  let dataset = [4; 8; 15; 16; 23; 42; 4; 8; 15; 16; 8] in

  Printf.printf "Dataset: %s\n\n" (string_of_int_list dataset);

  let m = mean dataset in
  let med = median dataset in
  let (modes, freq) = mode dataset in

  Printf.printf "Mean:   %.4f\n" m;
  Printf.printf "Median: %.4f\n" med;
  Printf.printf "Mode(s) (frequency = %d): %s\n" freq (string_of_int_list modes)

(import t f conj disj "bool-2.lisp")

(inspect-run
 (disj
  (conj (conj (t) (t))
        (conj (f) (t)))
  (disj (disj (t) (t))
        (disj (f) (t)))))

;QUESTION 1 
;takes a single integer argument N, and returns the Nth Padovan number
(defun PAD(N)
    ;The base cases for N=0, 1, or 2
    ;if got to the base cases, return 1
    (cond ((or (= N 0) (= N 1) (= N 2)) 1)
    ;otherwise recursively call PAD(N)= PAD(n - 2) + PAD(n - 3) since the hw is PAD(n + 1) = PAD(n - 1) + PAD(n - 2)
            (T (+ (PAD (- N 2)) (PAD (- N 3))))
    )
)
;---------------------------------------------------------------------------------------------------------------------
;QUESTION 2
;calls SUMS, that takes a single numeric argument N, and returns the number of additions 
;required by your PAD function to compute the Nth Padovan number.
;for the most part I do the same however, for the bases cases I return 0 and every time I do a call to SUMS, i return 1 which will be 
;added to 0 for every run
(defun SUMS(N)
    (cond ((or (= N 0) (= N 1) (= N 2)) 0)
    ;Do the same like PAD function except that in the recursive call, return 1 as each time call SUMS to increment the addition
            (T (+ (SUMS (- N 2)) (SUMS (- N 3)) 1))
    )
)
; ---------------------------------------------------------------------------------------------------------------------
;QUESTION 3
;; It takes a single argument TREE that represents a tree, and
;; returns an anonymized tree with the same structure, but where all symbols and numbers in the tree
;; are replaced by a question mark. The anonymized versions of the trees above are as follows.
(defun ANON(TREE)
    ;If the tree is empty return an empty list
    (cond ((null TREE) '())
    ;else if the tree contains a single leaf node L, it can be represented by atom L
    ;if Tree is atom, return ?
          ((atom TREE) '?)
    ;esle create a tree with cons that has ?; attach car and cdr together
          (T (cons (ANON (car TREE)) (ANON (cdr TREE))))
    )
)
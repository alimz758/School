;
; CS161 Hw3: Sokoban
; 
; *********************
;    READ THIS FIRST
; ********************* 
;
; All functions that you need to modify are marked with 'EXERCISE' in their header comments.
; Do not modify a-star.lsp.
; This file also contains many helper functions. You may call any of them in your functions.
;
; *Warning*: The provided A* code only supports the maximum cost of 4999 for any node.
; That is f(n)=g(n)+h(n) < 5000. So, be careful when you write your heuristic functions.
; Do not make them return anything too large.
;
; For Allegro Common Lisp users: The free version of Allegro puts a limit on memory.
; So, it may crash on some hard sokoban problems and there is no easy fix (unless you buy 
; Allegro). 
; Of course, other versions of Lisp may also crash if the problem is too hard, but the amount
; of memory available will be relatively more relaxed.
; Improving the quality of the heuristic will mitigate this problem, as it will allow A* to
; solve hard problems with fewer node expansions.
; 
; In either case, this limitation should not significantly affect your grade.
; 
; Remember that most functions are not graded on efficiency (only correctness).
; Efficiency can only influence your heuristic performance in the competition (which will
; affect your score).
;  
;


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; General utility functions
; They are not necessary for this homework.
; Use/modify them for your own convenience.
;

;
; For reloading modified code.
; I found this easier than typing (load "filename") every time. 
;
(defun reload()
  (load "hw3.lsp")
  )

;
; For loading a-star.lsp.
;
(defun load-a-star()
  (load "a-star.lsp"))

;
; Reloads hw3.lsp and a-star.lsp
;
(defun reload-all()
  (reload)
  (load-a-star)
  )

;
; A shortcut function.
; goal-test and next-states stay the same throughout the assignment.
; So, you can just call (sokoban <init-state> #'<heuristic-name>).
; 
;
(defun sokoban (s h)
  (a* s #'goal-test #'next-states h)
  )

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; end general utility functions
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
; We now begin actual Sokoban code
;

; Define some global variables
(setq blank 0)
(setq wall 1)
(setq box 2)
(setq keeper 3)
(setq star 4)
(setq boxstar 5)
(setq keeperstar 6)

; Some helper functions for checking the content of a square
(defun isBlank (v)
  (= v blank)
  )

(defun isWall (v)
  (= v wall)
  )

(defun isBox (v)
  (= v box)
  )

(defun isKeeper (v)
  (= v keeper)
  )

(defun isStar (v)
  (= v star)
  )

(defun isBoxStar (v)
  (= v boxstar)
  )

(defun isKeeperStar (v)
  (= v keeperstar)
  )

;
; Helper function of getKeeperPosition
;
(defun getKeeperColumn (r col)
  (cond ((null r) nil)
	(t (if (or (isKeeper (car r)) (isKeeperStar (car r)))
	       col
	     (getKeeperColumn (cdr r) (+ col 1))
	     );end if
	   );end t
	);end cond
  )

;
; getKeeperPosition (s firstRow)
; Returns a list indicating the position of the keeper (c r).
; 
; Assumes that the keeper is in row >= firstRow.
; The top row is the zeroth row.
; The first (right) column is the zeroth column.
;
(defun getKeeperPosition (s row)
  (cond ((null s) nil)
	(t (let ((x (getKeeperColumn (car s) 0)))
	     (if x
		 ;keeper is in this row
		 (list x row)
		 ;otherwise move on
		 (getKeeperPosition (cdr s) (+ row 1))
		 );end if
	       );end let
	 );end t
	);end cond
  );end defun

;
; cleanUpList (l)
; returns l with any NIL element removed.
; For example, if l is '(1 2 NIL 3 NIL), returns '(1 2 3).
;
(defun cleanUpList (L)
  (cond ((null L) nil)
	(t (let ((cur (car L))
		 (res (cleanUpList (cdr L)))
		 )
	     (if cur 
		 (cons cur res)
		  res
		 )
	     );end let
	   );end t
	);end cond
  );end 

; EXERCISE: Modify this function to return true (t)
; if and only if s is a goal state of a Sokoban game.
; (no box is on a non-goal square)
;
; Helper to see if there is any boxes on a row
(defun isBoxOnRow (List)
	; check if the list is empty then return empty
  (cond ((null List) ())
	(t (or (isBoxOnRow (car List)) (checkRow (cdr List))))
   )
)
; Currently, it always returns NIL. If A* is called with
; this function as the goal testing function, A* will never
; terminate until the whole search space is exhausted.
;
(defun goal-test (s)
	;if the final state is null return true
	(cond ((null s) t)
		;otherwise check if there are boxes on the row
		(t (and (not (isBoxOnRow (car s))) (goal-test (cdr s))))
	)
);end defun

; EXERCISE: Modify this function to return the list of 
; sucessor states of s.
;
; This is the top-level next-states (successor) function.
; Some skeleton code is provided below.
; You may delete them totally, depending on your approach.
; 
; If you want to use it, you will need to set 'result' to be 
; the set of states after moving the keeper in each of the 4 directions.
; A pseudo-code for this is:
; 
; ...
; (result (list (try-move s UP) (try-move s DOWN) (try-move s LEFT) (try-move s RIGHT)))
; ...
; 
; You will need to define the function try-move and decide how to represent UP,DOWN,LEFT,RIGHT.
; Any NIL result returned from try-move can be removed by cleanUpList.
; 
;
;;-------------- all helper function for next-state ------
; Helper function for get-square - rows
(defun getRow (S r)
        ; invalid
  (cond ((< r 0) ())
        ; if the state is null
        ((NULL S) ())
        ; if the first row
        ((= 0 r) (car S))
        ; recursivly check rows
        (T (getRow (cdr S) (- r 1)))
  )
)
; helper function for get-square  - col
(defun getCol (S c)
        ; if state is wall
  (cond ((NULL S) 1)
        ; if col is wall
	((< c 0) 1)
        ; check the fist col
	((= c 0) (car S))
        ; recursivly check cols
	(T (getCol (cdr S) (- c 1)))
  )
)

; gets the element at (r,c)
(defun get-square (S r c)
  (let ((element (getRow S r)))
       (getCol element c)
  )
)

; kakes in a state S, a row number r, a column number c, and a square content v
; (integer). Return a new state S’ that is obtained by setting the square (r,c)
; to value v. Does not modify input state.
(defun set-square (S r c v)
        ; Outside of state
  (cond ((or (null S) (null (car S))) S)
        ; Reached correct coordinate
        ((and (= r 0) (= c 0)) (cons (cons v (cdr (car S))) (cdr S)))
        ; Recurse through rows
        ((>= r 1) (cons (car S) (set-square (cdr S) (- r 1) c v)))
        ; Recurse through columns
        ((>= c 1) (let ((xs (set-square (cons (cdr (car S)) (cdr S))
                  r (- c 1) v))) (cons (cons (car (car S)) (car xs)) (cdr xs))
        ))
  )
)

; helper function for try-move. Attempts to move keeper. Takes state S, row r,
; column c, keeper k, element one move away from keeper a, and element two
; moves away from keeper b. Returns a new state.
(defun move (S r c k a b)
        ; Are we one move away from wall?
  (cond ((isWall a) ())
        ; Just pushed box; is the next element not goal or blank?
        ((and (isBox a) (not (or (isStar b) (isBlank b)))) ())
        ; move keeper
        ((= k keeper) (set-square S r c blank))
        ((= k keeperstar) (set-square S r c star))
  )
)

; A function takes in a state S and a move direction D. This function returns
; the state that is the result of moving the keeper in state S in direction D.
; it returns nil if the direction is invalid or can't not be made
(defun try-move (S D)
  ; get keeper position and element at coordinate
  (let* ((pos (getKeeperPosition S 0)) (c (first pos)) (r (first (rest pos)))
         (k (get-square S r c)))

    (cond
      ; Move up;  adjust 1, 2 step values
      ((equal D 'UP) (let* ((u1 (get-square S (- r 1) c)) (u2 (get-square S (- r 2) c)) (a (move S r c k u1 u2)))
        (cond ((isBlank u1) (set-square a (- r 1) c keeper))
              ((and (isBox u1) (isBlank u2)) (set-square (set-square a (- r 1) c keeper) (- r 2) c box))
              ((and (isBox u1) (isStar u2)) (set-square (set-square a (- r 1) c keeper) (- r 2) c boxstar))
              ((isStar u1) (set-square a (- r 1) c keeperstar))
              ((and (isBoxStar u1) (isBlank u2)) (set-square (set-square a (- r 1) c keeperstar) (- r 2) c box))
              ((and (isBoxStar u1) (isStar u2)) (set-square (set-square a (- r 1) c keeperstar) (- r 2) c boxstar))
        )
      ))

      ; Move down; adjust 1, 2 step values
      ((equal D 'DOWN) (let* ((d1 (get-square S (+ r 1) c)) (d2 (get-square S (+ r 2) c)) (a (move S r c k d1 d2)))
        (cond ((isBlank d1) (set-square a (+ r 1) c keeper))
              ((and (isBox d1) (isBlank d2)) (set-square (set-square a (+ r 1) c keeper) (+ r 2) c box))
              ((and (isBox d1) (isStar d2)) (set-square (set-square a (+ r 1) c keeper) (+ r 2) c boxstar))
              ((isStar d1) (set-square a (+ r 1) c keeperstar))
              ((and (isBoxStar d1) (isBlank d2)) (set-square (set-square a (+ r 1) c keeperstar) (+ r 2) c box))
              ((and (isBoxStar d1) (isStar d2)) (set-square (set-square a (+ r 1) c keeperstar) (+ r 2) c boxstar))
        )
      ))

      ; Move left; adjust 1, 2 step values
      ((equal D 'LEFT) (let* ((l1 (get-square S r (- c 1))) (l2 (get-square S r (- c 2))) (a (move S r c k l1 l2)))
        (cond((isBlank l1) (set-square a r (- c 1) keeper))
             ((and (isBox l1) (isBlank l2)) (set-square (set-square a r (- c 1) keeper) r (- c 2) box))
             ((and (isBox l1) (isStar l2)) (set-square (set-square a r (- c 1) keeper) r (- c 2) boxstar))
             ((isStar l1) (set-square a r (- c 1) keeperstar))
             ((and (isBoxStar l1) (isBlank l2)) (set-square (set-square a r (- c 1) keeperstar) r (- c 2) box))
             ((and (isBoxStar l1) (isStar l2)) (set-square (set-square a r (- c 1) keeperstar) r (- c 2) boxstar))
        )
      ))

      ; Move right; adjust 1, 2 step values
      ((equal D 'RIGHT) (let* ((r1 (get-square S r (+ c 1))) (r2 (get-square S r (+ c 2))) (a (move S r c k r1 r2)))
        (cond ((isBlank r1) (set-square a r (+ c 1) keeper))
              ((and (isBox r1) (isBlank r2)) (set-square (set-square a r (+ c 1) keeper) r (+ c 2) box))
              ((and (isBox r1) (isStar r2)) (set-square (set-square a r (+ c 1) keeper) r (+ c 2) boxstar))
              ((isStar r1) (set-square a r (+ c 1) keeperstar))
              ((and (isBoxStar r1) (isBlank r2)) (set-square (set-square a r (+ c 1) keeperstar) r (+ c 2) box))
              ((and (isBoxStar r1) (isStar r2)) (set-square (set-square a r (+ c 1) keeperstar) r (+ c 2) boxstar))
        )
      ))
    )
  )
)

(defun next-states (s)
	;try to move the four possible direction by using the try-move helper function
  (cleanUpList (list (try-move s 'UP) (try-move s 'DOWN) (try-move s 'LEFT) (try-move s 'RIGHT)))
);

; EXERCISE: Modify this function to compute the trivial 
; admissible heuristic.
;
(defun h0 (s)
;(5 pts) Write a heuristic function called h0 that returns the constant 0. This is a trivial admissible
	0
)

(defun h1 (s)
	; Reached end of s
    (cond ((null s) 0)
          ; check whether found a box
          ((atom s) (cond ((isBoxOnRow s) 1) 
		  			(T 0)))
          ; other recursively search
          (T (+ (h1 (car s)) (h1 (cdr s))))
    )
)
; EXERCISE: Modify this function to compute the 
; number of misplaced boxes in s.
;;------------------- All the helper functions written for h305179067  ------------------------
; simply calculate the abs value of the number passed to the function
(defun absVal (n)
  (cond ((>= n 0) n)
         (T (- 0 n))
  )
)

; Find the manhattan dist; it takes lists in the format (r1, c1, r2, 2).
(defun manhattanDist (l)
  (cond ((and (<= (- (car l) (third l)) 1) (<= (- (second l) (fourth l)) 1)) (+ (* (- (car l) (third l)) -1) (* (- (second l) (fourth l)) -1)))
         ((<= (- (car l) (third l)) 1) (+ (* (- (car l) (third l)) -1) (- (second l) (fourth l))))
         ((<= (- (second l) (fourth l)) 1) (+ (- (car l) (third l)) (* (- (second l) (fourth l)) -1)))
         (T (+ (- (car l) (third l)) (- (second l) (fourth l))))
  )
)

; find the location of the nearest box
(defun nearestBox (S l)
        ; if there is one box, return its location
  (cond ((equal (length l) 1) (list (manhattanDist (append (car l) (getKeeperPosition S 0))) (car l)))
        ; if there are two boxes then return the closer one's location
        ((equal (length l) 2)
          (cond ((<= (manhattanDist (append (car l) (getKeeperPosition S 0)))
                     (manhattanDist (append (second l) (getKeeperPosition S 0)))
                 )
                 (list (manhattanDist (append (car l) (getKeeperPosition S 0))) (car l))
                )
                (T (list (manhattanDist (append (second l) (getKeeperPosition S 0))) (second l)))
          )
        )
        ; otherwise, compare car of box list to rest of boxes
        (T (cond ((<= (manhattanDist (append (car l) (getKeeperPosition S 0)))
                      (car (nearestBox S (cdr l)))
                  )
                  (list (manhattanDist (append (car l) (getKeeperPosition s 0))) (car l))
                 )
                 (T (nearestBox s (cdr l)))
            )
         )
  )
)

; this function returns the locations of all boxes not yet at the goal;
(defun boxesLeft (S r c)
        ; null state
  (cond ((null S) ())
        ; if there is one element
        ((atom S) (cond ((isBoxOnRow S) (list (cons (- c 1) (cons r ()))))
                  (T ())))
        ; Recursively search state for box locations
        (T (cond ((= c 0) (append (boxesLeft (car S) r (+ c 1)) (boxesLeft (cdr S) (+ r 1) c)))
                 (T (append (boxesLeft (car S) r c) (boxesLeft (cdr S) r (+ c 1)))))
        )
  )
)

; Returns the locations of all goals not yet filled;.
(defun leftGoalsLocation (S r c)
        ; Null state
  (cond ((null S) ())
        ((atom S) (cond ((isStar S) (list (cons (- c 1) (cons r ()))))
                  (T ())))
        ; Recursively search state for box locations
        (T (cond ((= c 0) (append (leftGoalsLocation (car S) r (+ c 1)) (leftGoalsLocation (cdr S) (+ r 1) c)))
                  (T (append (leftGoalsLocation (car S) r c) (leftGoalsLocation (cdr S) r (+ c 1)))))
        )
  )
)

; to find the shortes path bw two points
(defun shortestPath (a b)
  (+ (absVal (- (car b) (car a))) (absVal (- (second b) (second a))))
)

; Shortest distance between any two points a and b
(defun shortestPath (a b curr)
  ; Endpoint is current point
  (cond ((null b) curr)
        (T (let ((path (shortestPath a (car b))))
                 ; Recursively search for shortest distance
                 (cond ((null curr) (shortestPath a (cdr b) path))
                   ; Did we find a shorter distance?
                   (t (cond ((>= path curr) (shortestPath a (cdr b) curr))
                            (t (shortestPath a (cdr b) path))))
                 )
           )
        )
  )
)

; sums the distances of each box to their nearest goal
(defun pathSum (box goal)
        ; No boxes
  (cond ((null goal) 0)
        ; Recursively sum path
        (T (+ (shortestPath (car box) goal ()) (pathSum (cdr box) (cdr goal))))
  )
)

; EXERCISE: Change the name of this function to h<UID> where
; <UID> is your actual student ID number. Then, modify this 
; function to compute an admissible heuristic value of s. 
; 
; This function will be entered in the competition.
; Objective: make A* solve problems as fast as possible.
; The Lisp 'time' function can be used to measure the 
; running time of a function call.
;
; Admissible heuristic with my UID
(defun h305179067 (s)
        ; null state
  (cond ((null s) 0)
        ; check whether  there any boxes left
        ((equal (length (boxesLeft s 0 0)) 0) 0)
        ; Sum distances
        (T (+ (car (nearestBox s (boxesLeft s 0 0))) (pathSum (boxesLeft s 0 0) (leftGoalsLocation s 0 0))))
  )
)




;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Some predefined problems.
 | Each problem can be visualized by calling (printstate <problem>). For example, (printstate p1).
 | Problems are roughly ordered by their difficulties.
 | For most problems, we also privide 2 additional number per problem:
 |    1) # of nodes expanded by A* using our next-states and h0 heuristic.
 |    2) the depth of the optimal solution.
 | These numbers are located at the comments of the problems. For example, the first problem below 
 | was solved by 80 nodes expansion of A* and its optimal solution depth is 7.
 | 
 | Your implementation may not result in the same number of nodes expanded, but it should probably
 | give something in the same ballpark. As for the solution depth, any admissible heuristic must 
 | make A* return an optimal solution. So, the depths of the optimal solutions provided could be used
 | for checking whether your heuristic is admissible.
 |
 | Warning: some problems toward the end are quite hard and could be impossible to solve without a good heuristic!
 | 
 |#

;(80,7)
(setq p1 '((1 1 1 1 1 1)
	   (1 0 3 0 0 1)
	   (1 0 2 0 0 1)
	   (1 1 0 1 1 1)
	   (1 0 0 0 0 1)
	   (1 0 0 0 4 1)
	   (1 1 1 1 1 1)))

;(110,10)
(setq p2 '((1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 1) 
	   (1 0 0 0 0 0 1) 
	   (1 0 0 2 1 4 1) 
	   (1 3 0 0 1 0 1)
	   (1 1 1 1 1 1 1)))

;(211,12)
(setq p3 '((1 1 1 1 1 1 1 1 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 2 0 3 4 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 0 0 0 1 0 0 0 1)
	   (1 1 1 1 1 1 1 1 1)))

;(300,13)
(setq p4 '((1 1 1 1 1 1 1)
	   (0 0 0 0 0 1 4)
	   (0 0 0 0 0 0 0)
	   (0 0 1 1 1 0 0)
	   (0 0 1 0 0 0 0)
	   (0 2 1 0 0 0 0)
	   (0 3 1 0 0 0 0)))

;(551,10)
(setq p5 '((1 1 1 1 1 1)
	   (1 1 0 0 1 1)
	   (1 0 0 0 0 1)
	   (1 4 2 2 4 1)
	   (1 0 0 0 0 1)
	   (1 1 3 1 1 1)
	   (1 1 1 1 1 1)))

;(722,12)
(setq p6 '((1 1 1 1 1 1 1 1)
	   (1 0 0 0 0 0 4 1)
	   (1 0 0 0 2 2 3 1)
	   (1 0 0 1 0 0 4 1)
	   (1 1 1 1 1 1 1 1)))

;(1738,50)
(setq p7 '((1 1 1 1 1 1 1 1 1 1)
	   (0 0 1 1 1 1 0 0 0 3)
	   (0 0 0 0 0 1 0 0 0 0)
	   (0 0 0 0 0 1 0 0 1 0)
	   (0 0 1 0 0 1 0 0 1 0)
	   (0 2 1 0 0 0 0 0 1 0)
	   (0 0 1 0 0 0 0 0 1 4)))

;(1763,22)
(setq p8 '((1 1 1 1 1 1)
	   (1 4 0 0 4 1)
	   (1 0 2 2 0 1)
	   (1 2 0 1 0 1)
	   (1 3 0 0 4 1)
	   (1 1 1 1 1 1)))

;(1806,41)
(setq p9 '((1 1 1 1 1 1 1 1 1) 
	   (1 1 1 0 0 1 1 1 1) 
	   (1 0 0 0 0 0 2 0 1) 
	   (1 0 1 0 0 1 2 0 1) 
	   (1 0 4 0 4 1 3 0 1) 
	   (1 1 1 1 1 1 1 1 1)))

;(10082,51)
(setq p10 '((1 1 1 1 1 0 0)
	    (1 0 0 0 1 1 0)
	    (1 3 2 0 0 1 1)
	    (1 1 0 2 0 0 1)
	    (0 1 1 0 2 0 1)
	    (0 0 1 1 0 0 1)
	    (0 0 0 1 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 4 1)
	    (0 0 0 0 1 1 1)))

;(16517,48)
(setq p11 '((1 1 1 1 1 1 1)
	    (1 4 0 0 0 4 1)
	    (1 0 2 2 1 0 1)
	    (1 0 2 0 1 3 1)
	    (1 1 2 0 1 0 1)
	    (1 4 0 0 4 0 1)
	    (1 1 1 1 1 1 1)))

;(22035,38)
(setq p12 '((0 0 0 0 1 1 1 1 1 0 0 0)
	    (1 1 1 1 1 0 0 0 1 1 1 1)
	    (1 0 0 0 2 0 0 0 0 0 0 1)
	    (1 3 0 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 2 1 1 1 0 0 0 1)
	    (1 0 0 0 0 1 0 1 4 0 4 1)
	    (1 1 1 1 1 1 0 1 1 1 1 1)))

;(26905,28)
(setq p13 '((1 1 1 1 1 1 1 1 1 1)
	    (1 4 0 0 0 0 0 2 0 1)
	    (1 0 2 0 0 0 0 0 4 1)
	    (1 0 3 0 0 0 0 0 2 1)
	    (1 0 0 0 0 0 0 0 0 1)
	    (1 0 0 0 0 0 0 0 4 1)
	    (1 1 1 1 1 1 1 1 1 1)))

;(41715,53)
(setq p14 '((0 0 1 0 0 0 0)
	    (0 2 1 4 0 0 0)
	    (0 2 0 4 0 0 0)	   
	    (3 2 1 1 1 0 0)
	    (0 0 1 4 0 0 0)))

;(48695,44)
(setq p15 '((1 1 1 1 1 1 1)
	    (1 0 0 0 0 0 1)
	    (1 0 0 2 2 0 1)
	    (1 0 2 0 2 3 1)
	    (1 4 4 1 1 1 1)
	    (1 4 4 1 0 0 0)
	    (1 1 1 1 0 0 0)
	    ))

;(91344,111)
(setq p16 '((1 1 1 1 1 0 0 0)
	    (1 0 0 0 1 0 0 0)
	    (1 2 1 0 1 1 1 1)
	    (1 4 0 0 0 0 0 1)
	    (1 0 0 5 0 5 0 1)
	    (1 0 5 0 1 0 1 1)
	    (1 1 1 0 3 0 1 0)
	    (0 0 1 1 1 1 1 0)))

;(3301278,76)
(setq p17 '((1 1 1 1 1 1 1 1 1 1)
	    (1 3 0 0 1 0 0 0 4 1)
	    (1 0 2 0 2 0 0 4 4 1)
	    (1 0 2 2 2 1 1 4 4 1)
	    (1 0 0 0 0 1 1 4 4 1)
	    (1 1 1 1 1 1 0 0 0 0)))

;(??,25)
(setq p18 '((0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 0 0 0 3 0 0 0 0 0 0 0)
	    (0 0 0 0 0 0 1 0 0 1 0 0 0 0 0 0)
	    (0 0 0 0 0 1 0 0 0 0 1 0 0 0 0 0)
	    (1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 0 1 0 0 0 0)
	    (0 0 0 0 1 0 0 0 0 0 4 1 0 0 0 0)
	    (0 0 0 0 1 0 2 0 0 0 0 1 0 0 0 0)	    
	    (0 0 0 0 1 0 2 0 0 0 4 1 0 0 0 0)
	    ))
;(??,21)
(setq p19 '((0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 0 1 0 0 1 0 0 0 0)
	    (0 0 0 0 0 0 3 0 0 0 2 0)
	    (0 0 0 0 1 0 0 1 0 0 0 4)
	    (1 1 1 1 0 0 0 0 1 1 1 1)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 0 0 0 1 0 0 0)
	    (0 0 0 1 0 2 0 4 1 0 0 0)))

;(??,??)
(setq p20 '((0 0 0 1 1 1 1 0 0)
	    (1 1 1 1 0 0 1 1 0)
	    (1 0 0 0 2 0 0 1 0)
	    (1 0 0 5 5 5 0 1 0)
	    (1 0 0 4 0 4 0 1 1)
	    (1 1 0 5 0 5 0 0 1)
	    (0 1 1 5 5 5 0 0 1)
	    (0 0 1 0 2 0 1 1 1)
	    (0 0 1 0 3 0 1 0 0)
	    (0 0 1 1 1 1 1 0 0)))

;(??,??)
(setq p21 '((0 0 1 1 1 1 1 1 1 0)
	    (1 1 1 0 0 1 1 1 1 0)
	    (1 0 0 2 0 0 0 1 1 0)
	    (1 3 2 0 2 0 0 0 1 0)
	    (1 1 0 2 0 2 0 0 1 0)
	    (0 1 1 0 2 0 2 0 1 0)
	    (0 0 1 1 0 2 0 0 1 0)
	    (0 0 0 1 1 1 1 0 1 0)
	    (0 0 0 0 1 4 1 0 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 0 1 4 0 1)
	    (0 0 0 0 1 4 4 4 0 1)
	    (0 0 0 0 1 1 1 1 1 1)))

;(??,??)
(setq p22 '((0 0 0 0 1 1 1 1 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 0 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 0 0 1 2 0 0 1 0 0 0 0 0 0 0 0 0 0)
	    (0 0 1 1 1 0 0 2 1 1 0 0 0 0 0 0 0 0 0)
	    (0 0 1 0 0 2 0 2 0 1 0 0 0 0 0 0 0 0 0)
	    (1 1 1 0 1 0 1 1 0 1 0 0 0 1 1 1 1 1 1)
	    (1 0 0 0 1 0 1 1 0 1 1 1 1 1 0 0 4 4 1)
	    (1 0 2 0 0 2 0 0 0 0 0 0 0 0 0 0 4 4 1)
	    (1 1 1 1 1 0 1 1 1 0 1 3 1 1 0 0 4 4 1)
	    (0 0 0 0 1 0 0 0 0 0 1 1 1 1 1 1 1 1 1)
	    (0 0 0 0 1 1 1 1 1 1 1 0 0 0 0 0 0 0 0)))

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

#|
 | Utility functions for printing states and moves.
 | You do not need to understand any of the functions below this point.
 |#

;
; Helper function of prettyMoves
; from s1 --> s2
;
(defun detectDiff (s1 s2)
  (let* ((k1 (getKeeperPosition s1 0))
	 (k2 (getKeeperPosition s2 0))
	 (deltaX (- (car k2) (car k1)))
	 (deltaY (- (cadr k2) (cadr k1)))
	 )
    (cond ((= deltaX 0) (if (> deltaY 0) 'DOWN 'UP))
	  (t (if (> deltaX 0) 'RIGHT 'LEFT))
	  );end cond
    );end let
  );end defun

;
; Translates a list of states into a list of moves.
; Usage: (prettyMoves (a* <problem> #'goal-test #'next-states #'heuristic))
;
(defun prettyMoves (m)
  (cond ((null m) nil)
	((= 1 (length m)) (list 'END))
	(t (cons (detectDiff (car m) (cadr m)) (prettyMoves (cdr m))))
	);end cond
  );

;
; Print the content of the square to stdout.
;
(defun printSquare (s)
  (cond ((= s blank) (format t " "))
	((= s wall) (format t "#"))
	((= s box) (format t "$"))
	((= s keeper) (format t "@"))
	((= s star) (format t "."))
	((= s boxstar) (format t "*"))
	((= s keeperstar) (format t "+"))
	(t (format t "|"))
	);end cond
  )

;
; Print a row
;
(defun printRow (r)
  (dolist (cur r)
    (printSquare cur)    
    )
  );

;
; Print a state
;
(defun printState (s)
  (progn    
    (dolist (cur s)
      (printRow cur)
      (format t "~%")
      )
    );end progn
  )

;
; Print a list of states with delay.
;
(defun printStates (sl delay)
  (dolist (cur sl)
    (printState cur)
    (sleep delay)
    );end dolist
  );end defun

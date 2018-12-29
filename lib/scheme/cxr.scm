#|
(scheme cxr) library
|#
(define-library (scheme cxr)
;  (import (scheme base))
  (import (sarcasm core))
  (export caar cadr cdar cddr
          caaar caadr cadar cdaar caddr cdadr cddar cdddr
          cddddr cadddr cdaddr cddadr cdddar caaddr cadadr)                
  (begin
    (define (caar lst) (car (car lst)))
    (define (cadr lst) (car (cdr lst)))
    (define (cdar lst) (cdr (car lst)))
    (define (cddr lst) (cdr (cdr lst)))
    (define (caaar lst) (car (car (car lst))))
    (define (caadr lst) (car (car (cdr lst))))
    (define (cadar lst) (car (cdr (car lst))))
    (define (cdaar lst) (cdr (car (car lst))))
    (define (caddr lst) (car (cdr (cdr lst))))
    (define (cdadr lst) (cdr (car (cdr lst))))
    (define (cddar lst) (cdr (cdr (car lst))))
    (define (cdddr lst) (cdr (cdr (cdr lst))))
    (define (cddddr lst) (cdr (cdr (cdr (cdr lst)))))
    (define (cadddr lst) (car (cdr (cdr (cdr lst)))))
    (define (cdaddr lst) (cdr (car (cdr (cdr lst)))))
    (define (cddadr lst) (cdr (cdr (car (cdr lst)))))
    (define (cdddar lst) (cdr (cdr (cdr (car lst)))))
    (define (caaddr lst) (car (car (cdr (cdr lst)))))
    (define (cadadr lst) (car (cdr (car (cdr lst)))))))


SELECT P.lastName, count(*)
FROM Persons AS P
GROUP BY P.lastName, P.firstName
HAVING count(P.firstName) > 1
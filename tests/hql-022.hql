SELECT P.firstName, P.lastName, Coll
FROM Persons AS P, PhotoCollection AS Coll
WHERE avg_cells(Coll) > 128
ORDER BY lastName

SELECT P.firstName, P.lastName, Coll
FROM Persons AS P, PhotoCollection AS Coll
WHERE avg_cells(Coll) > 128
LIMIT 50 OFFSET 10

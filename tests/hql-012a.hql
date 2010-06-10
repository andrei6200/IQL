SELECT P.firstName, P.lastName, Coll
FROM Persons AS P, PhotoCollection AS Coll
WHERE P.size < 5 * 1024 AND avg_cells(Coll) > 100

SELECT P.firstName, P.lastName, Coll
FROM Persons AS P, PhotoCollection AS Coll
WHERE P.size >= avg_cells(Coll)

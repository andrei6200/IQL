SELECT P.lastName, LEN(P.lastName), Coll
FROM Persons AS P, PhotoCollection AS Coll
WHERE avg_cells(Coll) > 128

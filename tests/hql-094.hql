SELECT P.lastName, LEN(P.lastName), 255 - Coll.red
FROM Persons AS P, PhotoCollection AS Coll

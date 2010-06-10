SELECT P.firstName, P.lastName, scale(Coll, [2,3])
FROM Persons AS P, PhotoCollection AS Coll
ORDER BY lastName

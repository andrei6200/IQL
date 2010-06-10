SELECT P.firstName, P.lastName, sdom(Coll), oid(Coll)
FROM Persons AS P, PhotoCollection AS Coll
ORDER BY lastName

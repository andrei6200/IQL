SELECT P.firstName, P.lastName, sdom(Coll), oid(Coll)
FROM Persons AS P, PhotoCollection AS Coll
LIMIT 50 OFFSET 10

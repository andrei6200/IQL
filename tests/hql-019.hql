SELECT P.firstName, P.lastName, sdom(Coll), oid(Coll)
FROM Persons AS P, PhotoCollection AS Coll
WHERE P.size < 5 * 1024

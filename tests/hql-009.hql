SELECT P.firstName, P.lastName, oid(Coll), sdom(Coll)
FROM Persons AS P, PhotoCollection AS Coll
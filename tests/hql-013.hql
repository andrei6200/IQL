SELECT P.firstName, P.lastName, Coll [1:100, 200:500]
FROM Persons AS P, PhotoCollection AS Coll
WHERE P.size < 5 * 1024

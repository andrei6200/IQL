SELECT P.firstName, P.lastName, JPEG(Coll * P.age)
FROM Persons AS P, PhotoCollection AS Coll
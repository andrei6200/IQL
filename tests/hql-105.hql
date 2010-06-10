SELECT P.firstName, P.lastName, scale(Coll, [2,3])
	INTO sqlTable
FROM Persons AS P, PhotoCollection AS Coll

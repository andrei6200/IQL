SELECT P.firstName, P.lastName, Coll 
	INTO sqlTable
FROM Persons AS P, PhotoCollection AS Coll

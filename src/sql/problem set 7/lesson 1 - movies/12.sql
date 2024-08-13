/*
In 12.sql, write a SQL query to list the titles of all movies in which both Bradley Cooper and Jennifer Lawrence starred.
Your query should output a table with a single column for the title of each movie.
You may assume that there is only one person in the database with the name Bradley Cooper.
You may assume that there is only one person in the database with the name Jennifer Lawrence.

Find the ID of Bradley Cooper
Find the ID of Jennifer Lawrence
Find the IDs of movies associated with Bradley Cooper’s ID
Find the IDs of movies associated with Jennifer Lawrence’s ID
Find movie titles from the movie IDs associated with both Bradley Cooper and Jennifer Lawrence
*/

--.schema

SELECT title, COUNT(title)
FROM movies
JOIN stars ON movies.id = stars.movie_id
WHERE stars.person_id IN
(
    SELECT id
    FROM people
    WHERE name = 'Bradley Cooper' OR name = 'Jennifer Lawrence'
)
GROUP BY title
ORDER BY count(title) DESC;

/*
SELECT COUNT(movie_id) AS count, movie_id
FROM stars
WHERE person_id IN
(
    SELECT id
    FROM people
    WHERE name = 'Bradley Cooper' OR name = 'Jennifer Lawrence'
)
GROUP BY movie_id
ORDER BY count DESC;
*/
/*
SELECT name
FROM people
WHERE name = 'Bradley Cooper' OR name = 'Jennifer Lawrence';
*/



-- select titles from movies
-- where movie.id is in stars
-- where people.name = Bradley Cooper & Jennifer Lawrence
/*
In 13.sql, write a SQL query to list the names of all people who starred in a movie in which Kevin Bacon also starred.
Your query should output a table with a single column for the name of each person.
There may be multiple people named Kevin Bacon in the database. Be sure to only select the Kevin Bacon born in 1958.
Kevin Bacon himself should not be included in the resulting list.
*/

-- Find the names of people with those people IDs
SELECT name
FROM people
WHERE id IN
(
    -- Find the IDs of people associated with those movie IDs
    SELECT person_id
    FROM stars
    WHERE movie_id IN
    (
        -- Find the IDs of movies associated with Kevin Bacon’s ID
        SELECT movie_id
        FROM stars
        WHERE person_id IN
        (
            -- Find the ID of Kevin Bacon (the one born in 1958!)
            SELECT id
            FROM people
            WHERE name = 'Kevin Bacon' AND birth = 1958
        )
    )
)
ORDER BY name;

/*




*/

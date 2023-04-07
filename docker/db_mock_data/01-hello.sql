CREATE TABLE IF NOT EXISTS hello ( 
    id INT NOT NULL AUTO_INCREMENT,
    name VARCHAR(255),
    date DATETIME, 
    PRIMARY KEY (id) 
);

INSERT INTO hello (name, date) 
VALUES 
    ( "Peter Griffin", "2023-01-01 12:00:00" ),
    ( "Raymond Lin", "2023-01-01 12:00:00" ),
    ( "Baxter Huntington", "2023-01-01 12:00:00" ),
    ( "Wilson Yu", "2023-01-01 12:00:00" ), 
    ( "Joey Woods", "2023-01-01 12:00:00" ), 
    ( "Jose Salinas", "2023-01-01 12:00:00" );

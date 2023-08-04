DROP DATABASE IF EXISTS my_game;
CREATE DATABASE IF NOT EXISTS my_game;

USE my_game;

-- Table for users
CREATE TABLE IF NOT EXISTS users(
	id_user INT AUTO_INCREMENT NOT NULL,
    user_name VARCHAR(25) NOT NULL,
    user_password_hash VARCHAR(20) NOT NULL, -- When creating a hash using <functional> a string of 20 maximum characters is produced on a 64 bits application.
    PRIMARY KEY(id_user)
);

-- Table for characters
CREATE TABLE IF NOT EXISTS characters(
	id_character INT AUTO_INCREMENT NOT NULL,
    character_name VARCHAR(25) NOT NULL,
    PRIMARY KEY(id_character)
);

-- Junction table between users and characters
CREATE TABLE IF NOT EXISTS users_characters(
	id_user INT NOT NULL,
    id_character INT NOT NULL,
    CONSTRAINT FK_users_characters_users_id_user
		FOREIGN KEY(id_user) REFERENCES users(id_user),
    CONSTRAINT FK_users_characters_characters_id_character
		FOREIGN KEY(id_character) REFERENCES characters(id_character),
    PRIMARY KEY(id_user, id_character)
);

-- Table for pet types (Had to name it "ptypes" because "types" is reserved and "_" is being used for junction tables)
CREATE TABLE IF NOT EXISTS ptypes(
	id_type INT AUTO_INCREMENT NOT NULL,
    type_name VARCHAR(25) NOT NULL,
    type_description VARCHAR(45) NOT NULL,
    PRIMARY KEY(id_type)
);

-- Table for pets
CREATE TABLE IF NOT EXISTS pets(
	id_pet INT AUTO_INCREMENT NOT NULL,
	pet_name VARCHAR(25) NOT NULL,
    pet_max_hp FLOAT NOT NULL,
    pet_current_hp FLOAT NOT NULL,
    pet_attack_damage FLOAT NOT NULL,
    id_type INT NOT NULL,
    CONSTRAINT FK_pets_ptypes_id_type
		FOREIGN KEY(id_type) REFERENCES ptypes(id_type),
	PRIMARY KEY(id_pet)
);

-- Junction Table between characters and pets
CREATE TABLE IF NOT EXISTS characters_pets(
	id_character INT NOT NULL,
    id_pet INT NOT NULL,
    CONSTRAINT FK_characters_pets_characters_id_character
		FOREIGN KEY(id_character) REFERENCES characters(id_character),
	CONSTRAINT FK_characters_pets_pets_id_pet
		FOREIGN KEY(id_pet) REFERENCES pets(id_pet),
    PRIMARY KEY(id_character, id_pet)
);

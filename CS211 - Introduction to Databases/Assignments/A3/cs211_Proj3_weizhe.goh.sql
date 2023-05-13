drop database if exists A3;
create database A3;
use A3;

-- Q0
create table Highschooler(ID int PRIMARY KEY, name text, grade int);
create table Friend(ID1 int, ID2 int, PRIMARY KEY(ID1, ID2), FOREIGN KEY(ID1) references Highschooler(ID), FOREIGN KEY(ID2) references Highschooler(ID))Engine = Innodb;
create table Likes(ID1 int, ID2 int, PRIMARY KEY(ID1, ID2), FOREIGN KEY(ID1) references Highschooler(ID), FOREIGN KEY(ID2) references Highschooler(ID))Engine = Innodb;
create table graduates(ID int PRIMARY KEY, name text, FOREIGN KEY(ID) references Highschooler(ID))Engine = Innodb;
-- Q1
delimiter //
create trigger T1 after insert on Highschooler
for each row
begin 
    insert into likes 
    select new.ID, ID from highschooler 
    where grade = new.grade and ID != new.ID and new.name = 'Friendly';
end //
delimiter ;
-- Q2
delimiter // 
create trigger T2 before insert on Highschooler
for each row 
begin 
    if(new.grade < 9 or new.grade > 12) then 
        set new.grade = 9;
    end if;
end //
delimiter ;
-- Q3
delimiter //
create trigger T3 before update on highschooler
for each row 
begin 
    if(new.grade > 12) then 
        delete from friend where friend.ID1 in (select ID from friend, highschooler where highschooler.ID = friend.ID1 and grade = old.grade) and ID2 = old.id;
        delete from friend where friend.ID2 in (select ID from friend, highschooler where highschooler.ID = friend.ID2 and grade = old.grade) and ID1 = old.id;
    end if;
end //
delimiter ;
-- Q4
delimiter //
create trigger T4 before update on highschooler 
for each row 
begin 
    if(new.grade > 12) then 
        insert into graduates values(new.ID, new.name); 
        set new.grade = -1;
    end if; 
end //
delimiter ;
        
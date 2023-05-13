drop database if exists FruitVegSrc;

/* Create database Sales of Fruit and Vegetable - FruitVegSrc */
create database FruitVegSrc;

/* Load the source database */
use FruitVegSrc

/* Create table for Sales data of Fruit and Vegetable */
create table sales(mon char(3), category varchar(10), product varchar(10), sales float, cost float,
                   primary key (mon, product));

/* Populate the tables with our data */
insert into sales values('Jan', 'Fruit', 'Apple', 120, 3),
                        ('Feb', 'Fruit', 'Apple', 200, 4),
                        ('Mar', 'Fruit', 'Apple', 210, 3),
                        ('Apr', 'Fruit', 'Apple', 180, 5),
                        ('May', 'Fruit', 'Apple', 170, 4),
                        ('Jun', 'Fruit', 'Apple', 150, 4),
                        ('Jul', 'Fruit', 'Apple', 100, 4),
                        ('Aug', 'Fruit', 'Apple', 110, 4.1),
                        ('Sep', 'Fruit', 'Apple', 180, 4.5),
                        ('Oct', 'Fruit', 'Apple', 190, 4),
                        ('Nov', 'Fruit', 'Apple', 210, 5),
                        ('Dec', 'Fruit', 'Apple', 210, 4.8),
                        ('Jan', 'Fruit', 'Pear', 120, 2),
                        ('Feb', 'Fruit', 'Pear', 200, 2.5),
                        ('Mar', 'Fruit', 'Pear', 210, 3),
                        ('Apr', 'Fruit', 'Pear', 80, 2),
                        ('May', 'Fruit', 'Pear', 70, 2),
                        ('Jun', 'Fruit', 'Pear', 50, 1.8),
                        ('Jul', 'Fruit', 'Pear', 100, 2.1),
                        ('Aug', 'Fruit', 'Pear', 120, 2.1),
                        ('Sep', 'Fruit', 'Pear', 70, 2.5),
                        ('Oct', 'Fruit', 'Pear', 190, 2.4),
                        ('Nov', 'Fruit', 'Pear', 180, 2.5),
                        ('Dec', 'Fruit', 'Pear', 100, 2.8),  
                        ('Jan', 'Vegetable', 'Cucumber', 80, 1),
                        ('Feb', 'Vegetable', 'Cucumber', 82, 1), 
                        ('Mar', 'Vegetable', 'Cucumber', 100, 0.8),
                        ('Apr', 'Vegetable', 'Cucumber', 90, 1.2),
                        ('May', 'Vegetable', 'Cucumber', 110, 1.2),
                        ('Jun', 'Vegetable', 'Cucumber', 105, 1.2),
                        ('Jul', 'Vegetable', 'Cucumber', 85, 1.1),
                        ('Aug', 'Vegetable', 'Cucumber', 80, 1.1),
                        ('Sep', 'Vegetable', 'Cucumber', 70, 1.5),
                        ('Oct', 'Vegetable', 'Cucumber', 90, 1.4),
                        ('Nov', 'Vegetable', 'Cucumber', 80, 1.2),
                        ('Dec', 'Vegetable', 'Cucumber', 100, 1.2),
                        ('Jan', 'Vegetable', 'Tomato', 100, 1.5),
                        ('Feb', 'Vegetable', 'Tomato', 90, 1.5),
                        ('Mar', 'Vegetable', 'Tomato', 100, 1.8),
                        ('Apr', 'Vegetable', 'Tomato', 90, 1.7),
                        ('May', 'Vegetable', 'Tomato', 110, 1.7),
                        ('Jun', 'Vegetable', 'Tomato', 105, 1.7),
                        ('Jul', 'Vegetable', 'Tomato', 95, 1.9),
                        ('Aug', 'Vegetable', 'Tomato', 90, 1.9),
                        ('Sep', 'Vegetable', 'Tomato', 90, 1.5),
                        ('Oct', 'Vegetable', 'Tomato', 90, 1.8),
                        ('Nov', 'Vegetable', 'Tomato', 90, 1.8),
                        ('Dec', 'Vegetable', 'Tomato', 100, 1.8),
                        ('May', 'Fruit', 'Durian', 50, 8),
                        ('Jun', 'Fruit', 'Durian', 150, 7.8),
                        ('Jul', 'Fruit', 'Durian', 145, 7.1),
                        ('Aug', 'Fruit', 'Durian', 120, 7.3),
                        ('Sep', 'Fruit', 'Durian', 80, 7);
/* ------------------------------------------------------------------------*/

drop database if exists FruitVegDes;

/* Create database where Cross table is in - FruitVegDes */
create database FruitVegDes;

use FruitVegDes

create table salesCross(category varchar(10), product varchar(10), Jan float, Feb float, Mar float,
                        Apr float, May float, Jun float, Jul float, Aug float, Sep float, Oct float,
                        Nov float, Dece float, grand_total float);
                        

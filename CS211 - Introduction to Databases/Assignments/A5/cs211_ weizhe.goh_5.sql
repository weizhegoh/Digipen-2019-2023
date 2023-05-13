create table Country(CountryName varchar(99) PRIMARY KEY) ENGINE = InnoDB;

create table Airport(AirportCode int PRIMARY KEY, AirportCountry varchar(99), AirportName varchar(99),
FOREIGN KEY(AirportCountry) REFERENCES Country(CountryName))ENGINE = InnoDB;

create table Airline(AirlineName varchar(99) PRIMARY KEY, AirlineCountry varchar(99),
FOREIGN KEY(AirlineCountry) REFERENCES Country(CountryName))ENGINE = InnoDB;

create table Flight(FlightNumber int PRIMARY KEY, AirlineName varchar(99), DepartureAirport int, DestinationAirport int, mileage int, 
DepartureTime time, DepartureDate date, DepartureDay varchar(99),
ArrivalTime time, ArrivalDate date, ArrivalDay varchar(99),
FOREIGN KEY(AirlineName) REFERENCES Airline(AirlineName),
FOREIGN KEY(DepartureAirport) REFERENCES Airport(AirportCode),
FOREIGN KEY(DestinationAirport) REFERENCES Airport(AirportCode))ENGINE = InnoDB;

create table Passenger(PassportNumber int PRIMARY KEY, PassengerName varchar(99), PassengerCountry varchar(99),
FOREIGN KEY(PassengerCountry) REFERENCES Country(CountryName))ENGINE = InnoDB;

create table TravelHistory(PassportNumber int, FlightNumber int, 
FOREIGN KEY(PassportNumber) REFERENCES Passenger(PassportNumber), FOREIGN KEY(FlightNumber) REFERENCES Flight(FlightNumber))ENGINE = InnoDB;
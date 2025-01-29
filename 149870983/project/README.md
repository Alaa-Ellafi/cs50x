# EcoPrint: Carbon Footprint Calculator
#### Video Demo:  <https://www.youtube.com/watch?v=jfWHnBXUUgA>
#### Description:

EcoPrint is a web application designed to help users calculate, track, and reduce their carbon footprint. It provides a simple and intuitive way to input daily activities like electricity consumption, transportation, fuel usage, and waste production to estimate an individual’s environmental impact. By making this information easily accessible, EcoPrint encourages users to take meaningful steps toward reducing their carbon emissions.

## Features:
### 1. User Authentication:
Users can register for an account or log in to an existing one. EcoPrint stores user data securely, allowing users to track their past calculations.

### 2. Carbon Footprint Calculator:
Users input various parameters such as:
- Electricity consumption (in kilowatt-hours)
- Fuel consumption (in gallons) for both gasoline and diesel
- Mileage driven
- Waste produced (in pounds)

The calculator uses EPA emission factors to compute the total carbon emissions, which are then displayed in metric tons of CO2.

### 3. Tracking:
EcoPrint provides a personalized *Track* page where users can view their past calculations in a graphical format, helping them visualize changes in their carbon footprint over time.

### 4. Insights and Air Quality:
EcoPrint integrates with the EPA AirNow API to offer real-time air quality data based on the user's zip code. The app provides actionable insights and recommendations based on the Air Quality Index (AQI), advising users on how to adjust their activities depending on the air quality conditions.

### 5. Simple and User-Friendly Design:
The web app features a clean and responsive interface built using Flask on the back-end and Bootstrap for the front-end. It is optimized for desktop and mobile use.

## Technical Details:
- **Back-End:** Flask (Python)
- **Database:** SQLite for storing user information, past calculations, and tracking data
- **Front-End:** HTML, CSS (Bootstrap)
- **API Integration:** AirNow API to fetch real-time air quality data
- **Calculation Logic:** Uses emission factors from the EPA's Greenhouse Gas Inventory to estimate the carbon footprint

## How it Works:
1. **User Registration/Login:** Users start by creating an account or logging in to their existing account.
2. **Input Data:** Users enter their daily activity data—electricity usage, fuel consumption, mileage, and waste.
3. **Calculation:** The app calculates the total emissions for each category and returns a carbon footprint estimate in metric tons.
4. **Track Progress:** Users can view and track their historical data to see trends in their carbon emissions.
5. **Air Quality Insights:** Users can also get real-time air quality information based on their location, and receive personalized recommendations.

## Challenges and Learnings:
Developing EcoPrint involved challenges related to user authentication, database management, and API integration. One of the key learnings was ensuring accurate calculation of emissions based on real-world factors, such as the type of fuel used. Handling the integration with the AirNow API and ensuring that real-time data was presented in an easy-to-understand format was another critical aspect of the project.

## Future Improvements:
In future iterations, EcoPrint could incorporate additional categories like food consumption, water usage, and travel habits to provide a more comprehensive view of an individual's carbon footprint. Moreover, implementing data visualization improvements and deeper analytics could enhance user engagement.

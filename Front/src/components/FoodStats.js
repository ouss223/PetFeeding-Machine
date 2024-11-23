import React, { useState, useEffect } from "react";
import {
    PieChart,
    Pie,
    Cell,
    Tooltip,
    Legend,
    ResponsiveContainer,
} from "recharts";
import { ClipLoader } from "react-spinners"; // Import the spinner
import { FaPaw } from 'react-icons/fa';
const ESP32_API = "http://localhost:8180";

const FoodStats = () => {
    const [weight, setWeight] = useState(null);
    const [loading, setLoading] = useState(true);
    const [error, setError] = useState(null);

    // Constants for nutritional values per 100g
    const caloriesPer100g = 100;
    const proteinPer100g = 10;
    const fatPer100g = 5;
    const carbsPer100g = 20;

    // Default cat's daily calorie requirement
    const dailyCalories = 250; // Example: a cat needs 250 calories a day

    const fetchWeight = async () => {
        try {
            const response = await fetch(`${ESP32_API}/getWeight`);
            if (!response.ok) throw new Error(`Error: ${response.statusText}`);
            const data = await response.json();
            setWeight(data.eatenFood * 1000);
        } catch (err) {
            setError(err.message);
        } finally {
            setLoading(false);
        }
    };

    useEffect(() => {
        fetchWeight();
    }, []);

    const calculateNutrients = (eatenFood) => {
        const totalCalories = (eatenFood / 100) * caloriesPer100g;
        const totalProtein = (eatenFood / 100) * proteinPer100g;
        const totalFat = (eatenFood / 100) * fatPer100g;
        const totalCarbs = (eatenFood / 100) * carbsPer100g;

        return { totalCalories, totalProtein, totalFat, totalCarbs };
    };

    if (loading) {
        return (
                <div className="text-2xl font-bold text-center mt-20">
                    <ClipLoader size={50} color={"#fff"} loading={true} />
                    <p className="mt-4">Loading Pet Nutrition Overview...</p>
                </div>
        );
    
    }

    if (error) {
        return <div>Error: {error}</div>;
    }
    if (weight==0 || weight==null) {
        return (
            <div className="bg-white shadow-xl rounded-xl p-8 w-full max-w-md text-center space-y-6 flex flex-col items-center mt-12">
                 <h1 className="text-3xl font-extrabold text-blue-600 mb-4 ">Pet Nutrition Overview</h1>
                <FaPaw className="text-blue-500 mr-2 " />
                <span className="text-blue-700 font-semibold">Your pet hasn't eaten yet today.</span>
            </div>
        );
    
    }

    const { totalCalories, totalProtein, totalFat, totalCarbs } = weight
        ? calculateNutrients(weight)
        : { totalCalories: 0, totalProtein: 0, totalFat: 0, totalCarbs: 0 };

    return (
        <div className="p-6 mt-14">
            <h1 className="text-3xl font-bold mb-4">Pet Nutrition Overview</h1>
            <p className="mb-6">
                Track the nutritional intake of your pet, including calories, protein,
                fat, and carbs.
            </p>

            {/* Pie Chart for Nutrients */}
            <ResponsiveContainer width="100%" height={300}>
                <PieChart>
                    <Pie
                        data={[
                            { name: "Protein", value: totalProtein },
                            { name: "Fat", value: totalFat },
                            { name: "Carbs", value: totalCarbs },
                        ]}
                        dataKey="value"
                        nameKey="name"
                        cx="50%"
                        cy="50%"
                        outerRadius={100}
                        label
                    >
                        <Cell key="protein" fill="#8884d8" />
                        <Cell key="fat" fill="#82ca9d" />
                        <Cell key="carbs" fill="#ffc658" />
                    </Pie>
                    <Tooltip />
                    <Legend />
                </PieChart>
            </ResponsiveContainer>

            {/* Progress Bar for Calories Consumed */}
            <div className="mt-8">
                <h3 className="text-xl font-medium mb-2">
                    Calories Consumed: {totalCalories.toFixed(2)} kcal
                </h3>
                <div className="relative pt-1">
                    <div className="flex mb-2 items-center justify-between">
                        <div>
                            <span className="font-semibold text-sm">{`${Math.round(
                                (totalCalories / dailyCalories) * 100
                            )}%`}</span>
                        </div>
                    </div>
                    <div className="flex mb-2 items-center justify-between">
                        <div className="w-full bg-gray-200 rounded-full">
                            <div
                                className="bg-green-500 text-xs font-medium text-center p-0.5 leading-none rounded-l-full"
                                style={{ width: `${Math.min((totalCalories / dailyCalories) * 100, 100)}%` }}
                            >
                                <span className="absolute inset-0 text-center text-white">{`${totalCalories.toFixed(
                                    0
                                )} / ${dailyCalories} kcal`}</span>
                            </div>
                        </div>
                    </div>
                </div>
            </div>
        </div>
    );
};

export default FoodStats;

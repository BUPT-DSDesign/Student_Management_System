import axios from 'axios'

export const service = axios.create({
    baseUrl: "localhost:8080/Student_Management_System/",
    timeout: 30000
})
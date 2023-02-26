import axios from 'axios'

export const service = axios.create({
    baseURL: "http://localhost:8080/Student_Management_System",
    timeout: 30000
})
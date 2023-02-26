import axios from 'axios'

export const service = axios.create({
    baseUrl: "",
    timeout: 30000
})
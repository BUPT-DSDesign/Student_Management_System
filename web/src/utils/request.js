import axios from 'axios'
import { useUserStore } from '@/pinia/modules/user'

export const service = axios.create({
    baseURL: "http://localhost:8080/Student_Management_System",
    timeout: 30000
})

// http request 拦截器
service.interceptors.request.use(
    config => {
        const userStore = useUserStore()
        config.headers = {
            'Content-Type': 'application/json',
            'token': window.localStorage.getItem('token')
            ...config.headers
        }
        return config
    },
    error => {
        ElMessage({
            showClose: true,
            message: error,
            type: 'error'
        })
        return error
    }
)

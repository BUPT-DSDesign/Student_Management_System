import axios from 'axios'
import { useUserStore } from '@/pinia/modules/user'
import { useNavigateStore } from '@/pinia/modules/navigate'

export const service = axios.create({
    baseURL: "http://localhost:8080/Student_Management_System",
    timeout: 30000
})

// http request 拦截器
service.interceptors.request.use(
    config => {
        config.headers = {
            // 'Access - Control - Allow - Headers': 'token,content-type',
            'Content-Type': 'application/json',
            'token': window.localStorage.getItem('token'),
            ...config.headers
        }
        return config
    },
    error => {
       
        return error
    }
)

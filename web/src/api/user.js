import { service } from '@/utils/request'


export const login = (data) => {
    return service({
        url: "http://localhost:8080/Student_Management_System/user/login",
        method: 'post',
        data: data
    })
}


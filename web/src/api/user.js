import { service } from '@/utils/request'

// 登录
export const Login = (data) => {
    return service({
        url: "/user/login",
        method: 'post',
        data: data
    })
}


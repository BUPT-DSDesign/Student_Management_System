import { service } from '@/utils/request'

// 登录
export const Login = (data) => {
    return service({
        url: "/user/login",
        method: 'post',
        data: data
    })
}

// 获取用户信息
export const GetInfo = () => {
    return service({
        url: "user/info",
        method: 'get'
    })
}

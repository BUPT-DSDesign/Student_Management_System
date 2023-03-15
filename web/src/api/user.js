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

// 编辑个性签名
export const EditSignature = (signature) => {
    const url = '/user/edit_signature?' + 'signature=' + signature
    return service({
        url: url,
        method: 'put'
    })
}


import {login} from '@/api/user'


export const LoginIn = async (loginInfo) => {
    const res = await login(loginInfo)
    console.log(res)
}
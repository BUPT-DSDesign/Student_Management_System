import { Register, Login, GetInfo, EditSignature } from '@/api/user'
import { defineStore } from 'pinia'
import { ref } from 'vue'
import { Loading } from 'element-ui'

export const useUserStore = defineStore('user', () => {
    const loadingInstance = ref(null) // 加载框
    
    // 注册
    const RegisterIn = async (registerInfo) => {
        loadingInstance.value = Loading.service({
            lock: true,
            text: '注册中，请稍候.....',
            background: 'rgba(0, 0, 0, 0.7)'
        })
        try {
            const res = await Register(registerInfo)
            if (res.data.status_code == 0) {
                initTokenAndId()
                // 注册成功后把token和userId存起来
                window.localStorage.setItem('token', res.data.token)
                window.localStorage.setItem('userId', res.data.user_id)

                loadingInstance.value.close()
                // ....其他操作
                return true
            }
            return false
        } catch (err) {
            loadingInstance.value.close()
            return false
        }
        loadingInstance.value.close()
    }


    // 登录
    // 初始化token, 用户每次登录的时候需要把原先的token清空
    const initTokenAndId = () => {
        window.localStorage.removeItem('token') //移除token
        window.localStorage.removeItem('userId') //移除id
    }
    const LoginIn = async (loginInfo) => {
        loadingInstance.value = Loading.service({
            lock: true,
            text: '登录中，请稍候.....',
            background: 'rgba(0, 0, 0, 0.7)'
        })
        try {
            const res = await Login(loginInfo)
            if (res.data.status_code == 0) {
                initTokenAndId()
                // 登录成功后把token和userId存起来
                window.localStorage.setItem('token', res.data.token)
                window.localStorage.setItem('userId', res.data.user_id)
                let role = res.data.is_admin == false ? "admin" : "user";
                localStorage.setItem("role", role);
                loadingInstance.value.close()
                // ....其他操作
                return true
            }

            return false
        } catch (err) {
            loadingInstance.value.close()
            return false
        }
        loadingInstance.value.close()
    }

    // 获取用户信息
    const userInfo = ref({})
    const GetUserInfo = async () => {
        try {
            const res = await GetInfo()
            if (res.data.status_code == 0) {
                userInfo.value = res.data.user_info
                return true
            }
        } catch (err) {
            return false
        }
    }

    // 编辑个性签名
    const EditUserSignature = async (signature) => {
        try {
            const res = await EditSignature(signature)

            if (res.data.status_code == 0) {
                console.log(res.data)
                return true
            }
        } catch (err) {
            return false
        }
    }

    return {
        RegisterIn,
        LoginIn,
        userInfo,
        GetUserInfo,
        EditUserSignature
    }
},{
    persist: true
})

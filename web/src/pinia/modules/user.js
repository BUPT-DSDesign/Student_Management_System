import { Login, GetInfo } from '@/api/user'
import { defineStore } from 'pinia'
import { ref } from 'vue'
import { Loading } from 'element-ui'

export const useUserStore = defineStore('user', () => {
    const loadingInstance = ref(null) // 登录时的加载框
    // 初始化token, 用户每次登录的时候需要把原先的token清空
    const initTokenAndId = () => {
        window.localStorage.removeItem('token') //移除token
        window.localStorage.removeItem('userId') //移除id
    }

    // 登录
    const LoginIn = async (loginInfo) => {
        loadingInstance.value = Loading.service({
            lock: true,
            text: '登录中，请稍候.....',
            background: 'rgba(0, 0, 0, 0.7)'
        })
        try {
            const res = await Login(loginInfo)
            // console.log()

            if (res.data.status_code == 0) {
                initTokenAndId()
                // 登录成功后把token存起来
                window.localStorage.setItem('token', res.data.token)
                window.localStorage.setItem('userId', res.data.user_id)

                loadingInstance.value.close()
                // ....其他操作
                return true
            }
        } catch (err) {
            loadingInstance.value.close()
            return false
        }
        loadingInstance.value.close()
    }

    // 获取用户信息
    const userInfo = ref(null)
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

    return {
        loadingInstance,
        LoginIn,
        userInfo,
        GetUserInfo
    }
})

import {Login} from '@/api/user'
import {defineStore} from 'pinia'
import { ref } from 'vue'
// import { ElLoading } from 'element-ui/types/loading'

export const useUserStore = defineStore('user', () => {
    // const loadingInstance = ref(null) // 登录时的加载框
    // const token = ref(window.localStorage.getItem('token') || '')

    // 初始化token, 用户每次登录的时候需要把原先的token清空
    const InitToken = () => {
        token.value = ''
        window.localStorage.removeItem('token')
        localStorage.clear()
    }

    // const LoginIn = async (loginInfo) => {
        // loadingInstance.value = ElLoading.service({
        //     fullscreen: true,
        //     text: '登录中，请稍候...',
        // })
        try {
            const res = await Login(loginInfo)
            console.log(res.data)
            if (res.data.status_code === 0) {
                InitToken()
                // 登录成功后把token存起来
                // token.value = res.data.token
                window.localStorage.setItem('token', res.data.token)
                console.log(window.localStorage.getItem('token'))
                // ....其他操作
                return true
            }
        } catch (err) {
            // loadingInstance.value.close()
            return false
        }
        // loadingInstance.value.close()
    }

    return {
        LoginIn
    }
})
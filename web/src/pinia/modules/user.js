import { Login } from '@/api/user'
import { defineStore } from 'pinia'
import { ref } from 'vue'
import { Loading } from 'element-ui'

export const useUserStore = defineStore('user', () => {
    const rdata = ref(null)

    const loadingInstance = ref(null) // 登录时的加载框
    // const token = ref(window.localStorage.getItem('token') || '')

    // 初始化token, 用户每次登录的时候需要把原先的token清空
    const initToken = () => {
        window.localStorage.removeItem('token')
        localStorage.clear()
    }

    const LoginIn = async (loginInfo) => {
        loadingInstance.value = Loading.service({
            lock: true,
            text: '登录中，请稍候.....',
            background: 'rgba(0, 0, 0, 0.7)'
        })
        try {
            const res = await Login(loginInfo)
            rdata.value = res.data
            console.log(rdata.value)
            // console.log()

            if (res.data.status_code == 0) {
                initToken()
                // 登录成功后把token存起来
                window.localStorage.setItem('token', res.data.token)

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

    return {
        rdata,
        loadingInstance,
        LoginIn
    }
})
